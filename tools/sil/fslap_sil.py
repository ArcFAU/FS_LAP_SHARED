"""The bridge between FS Lap's SIL mode and a VCU: FS Lap sends its driver and car state over UDP, the VCU's inverter
limits come back, and FS Lap's ADTM is driven by the inverters' speed loop between those limits (FSLapSil.cpp).

    python tools\\sil\\fslap_sil.py --stand-in                 no firmware: a plain torque request (tests the loop)
    python tools\\sil\\fslap_sil.py --dll <vcu_full.dll>       the car's VCU firmware through the team's SIL

Start it, then choose SIL in FS Lap's menu. FS Lap sends to 127.0.0.1:8771 (--port) and gets the answers on the
port it sent from.

With --dll the team's own closed loop runs (vehicle-controls\\03_SIL\\vcu_full\\closed_loop_vcu.py, read from
--sil, never changed): the virtual start (virtual_log.py: BMS, TS, starter, no car log), the Ephorus inverter
emulator and every CAN, IMU and pedal frame the VCU takes. Only its vehicle model and driver are FS Lap: the IMU
packets, motor speeds and torques come from FS Lap's ADTM, the pedals and steering from the driver in FS Lap. The
firmware runs paced to the wall clock once the loop is closed. vcu_full.dll is built by vcu_full\\build.sh from the
team's software repository (D:\\FSLap_VD\\software).
shortcut: --dll has not been run yet, because no vcu_full.dll exists on this PC (2026-11-15); run it once the DLL is
built and fix what the first run shows.

Packets (little endian), wheels in FS Lap's order FL, FR, RL, RR:
  FS Lap -> bridge  "FSL1", seq u32, flags u32 (1: driving), then 18 doubles: sim time s, throttle 0..1, brake 0..1,
                    steering wheel deg (+ right, as the car logs it), vx, vy m/s, yaw rate rad/s, ax, ay m/s2 (vy, yaw
                    rate and ay + left, as the VCU's IMU), heading deg (clockwise), motor rpm x4, motor torque Nm x4
  bridge -> FS Lap  "VCU1", seq u32 (FS Lap's last), flags u32 (1: firmware, 2: stand-in), then 13 doubles: VCU time s,
                    speed setpoint rpm x4, torque limit up Nm x4, torque limit down Nm x4; then 4 int32: inverter state
                    (0 safe, 1 reset, 2 enable, 3 drive; -1 unknown)
"""
import argparse
import os
import socket
import struct
import sys
import time
import functools

print = functools.partial(print, flush=True)   # the log shows each line at once, also when the bridge is stopped

FSL = struct.Struct("<4sII18d")
VCU = struct.Struct("<4sII13d4i")
DRIVE = 3
T_MAX = 18.8          # Nm, the inverters' own limit (vehicle_model/vehicle.py, the logs)
P_MAX = 80e3          # W, the stand-in's system power


class Link:
    """UDP on localhost; keeps FS Lap's newest packet"""

    def __init__(self, port):
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.sock.bind(("127.0.0.1", port))
        self.sock.setblocking(False)
        self.peer = None
        self.state = None     # the newest FS Lap packet as a dict
        self.seq = 0

    def poll(self, wait=0.0):
        if wait > 0:
            self.sock.settimeout(wait)
            self.sock.setblocking(True)
        try:
            while True:
                data, self.peer = self.sock.recvfrom(1024)
                if len(data) == FSL.size and data[:4] == b"FSL1":
                    f = FSL.unpack(data)
                    d = f[3:]
                    self.seq = f[1]
                    self.state = dict(flags=f[2], t=d[0], thr=d[1], brk=d[2], steer=d[3], vx=d[4], vy=d[5], r=d[6], ax=d[7],
                                      ay=d[8], heading=d[9], rpm=d[10:14], torque=d[14:18])
                self.sock.setblocking(False)
        except (BlockingIOError, socket.timeout):
            pass
        except ConnectionResetError:   # Windows reports FS Lap's closed port this way
            pass
        finally:
            self.sock.setblocking(False)
        return self.state

    def send(self, t_vcu, n_set, t_pos, t_neg, states, flags):
        if self.peer is not None:
            self.sock.sendto(VCU.pack(b"VCU1", self.seq, flags, t_vcu, *n_set, *t_pos, *t_neg, *states), self.peer)


def stand_in(link):
    """no firmware: the throttle asks for torque up to 18.8 Nm per motor within 80 kW, the inverters are always in
    Drive, no regeneration. Only for testing FS Lap's side of the loop."""
    print("stand-in VCU on port", link.sock.getsockname()[1], "- choose SIL in FS Lap's menu; Ctrl+C ends")
    t0 = time.time()
    seen = False
    while True:
        s = link.poll(wait=0.05)
        if s is None:
            continue
        if not seen:
            print("FS Lap connected")
            seen = True
        w = [abs(n) * 2 * 3.141592653589793 / 60 for n in s["rpm"]]
        t_up = min(1.0, max(0.0, s["thr"])) * T_MAX
        power = t_up * sum(w)
        if power > P_MAX:
            t_up *= P_MAX / power
        link.send(time.time() - t0, [20000.0] * 4, [t_up] * 4, [0.0] * 4, [DRIVE] * 4, 2)


def firmware(link, dll, sil, minutes):
    """the team's closed loop with FS Lap as its vehicle model and driver"""
    import numpy as np
    sys.path[:0] = [os.path.join(sil, "vcu_full"), os.path.join(sil, "vehicle_model")]
    import closed_loop_vcu as cl
    import ephorus_sim as es
    import virtual_log
    from params import VEH
    from vehicle import RPM

    gear = VEH["i_gear"]

    class FSLapDriver:
        """stands in for the team's PathDriver: FS Lap's driver (throttle 0..999, steering wheel deg, brake bar)"""
        p = {"t_track": 60.0 * minutes}
        ms0 = 0

        def __call__(self, dt, veh):
            s = link.state or dict(thr=0.0, brk=0.0, steer=0.0)
            # shortcut: FS Lap has a brake pedal, not pressures; 40 / 17 bar at full pedal is a guess near the logs' range
            return float(np.clip(s["thr"], 0, 1)) * 999.0, s["steer"], (40.0 * s["brk"], 17.0 * s["brk"])

    class FSLapPlant(cl.Plant):
        """the team's Plant with FS Lap in place of its vehicle model: the VCU's limits go out, FS Lap's state comes in"""
        wall0 = None

        def __call__(self, ms, eph):
            n_set, t_pos, t_neg, st = [0.0] * 4, [0.0] * 4, [0.0] * 4, [-1] * 4
            for i, inv in enumerate(eph.inv):
                j = cl.J[i]
                st[j] = int(getattr(inv, "state", -1))
                if inv.lim is not None:
                    n_set[j], t_neg[j], t_pos[j] = inv.lim
            if ms < self.ms_on:   # the VCU's start-up, as fast as it goes; FS Lap shows the states
                if ms % 50 == 0:
                    link.poll()
                    link.send(ms / 1000.0, n_set, t_pos, t_neg, st, 1)
                return
            if not self.on:
                self.start(ms)
                self.wall0 = time.time() - ms / 1000.0
            lag = ms / 1000.0 - (time.time() - self.wall0)
            if lag > 0:
                time.sleep(lag)     # paced: FS Lap's driver is in the loop
            s = link.poll()
            link.send(ms / 1000.0, n_set, t_pos, t_neg, st, 1)
            if s is None:
                return
            v = self.veh
            v.vx, v.vy, v.r = s["vx"], s["vy"], s["r"]
            v.omega = np.array(s["rpm"]) / gear / RPM
            self.ax, self.ay, self.psi = s["ax"], s["ay"], s["heading"]
            if self.driver is not None and (self.d_out is not None or any(inv.state == es.DRIVE for inv in eph.inv)):
                if self.d_out is None:
                    self.driver.ms0 = ms
                self.d_out = self.driver(1e-3, v)
            for i, inv in enumerate(eph.inv):
                inv.torque = inv.torque_set = float(s["torque"][cl.J[i]])

    over = {"length": virtual_log.PARAMS["t_ts"] + 60.0 * minutes + 90.0}
    log, p = virtual_log.make(over)
    t0, t1 = p["t0"], p["t0"] + 60.0 * minutes
    cl.Plant = FSLapPlant
    cl.PathDriver = lambda path, beta0=0.0: FSLapDriver()
    print(f"VCU firmware {dll}, {minutes:g} min; waiting for FS Lap on port {link.sock.getsockname()[1]}")
    while link.poll(wait=0.5) is None:
        pass
    print("FS Lap connected; the VCU starts (BMS, TS, starter), then the loop closes")
    cl.run(log, t0, t1, dll, driver="path", track="endurance")


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    ap.add_argument("--port", type=int, default=8771)
    g = ap.add_mutually_exclusive_group(required=True)
    g.add_argument("--stand-in", action="store_true", help="no firmware: a plain torque request")
    g.add_argument("--dll", help="vcu_full.dll from vehicle-controls\\03_SIL\\vcu_full\\build.sh")
    ap.add_argument("--sil", default=r"D:\FSLap_VD\vehicle-controls\03_SIL", help="the team's SIL (only read)")
    ap.add_argument("--minutes", type=float, default=30.0, help="with --dll: how long the session can last")
    a = ap.parse_args()
    link = Link(a.port)
    try:
        stand_in(link) if a.stand_in else firmware(link, os.path.abspath(a.dll), a.sil, a.minutes)
    except KeyboardInterrupt:
        pass


if __name__ == "__main__":
    main()
