"""Compares the car's logged test days with the simulator: standing starts, braking, cornering and the powertrain.

    python tools\\logs\\validate.py

Reads the logs listed in LOGS (decoding each once with asc_decode.py into data\\logs), runs data\\logs\\sim_accel.exe
(build it with tools\\logs\\sim_accel.bat) and reads the AI lap traces data\\logs\\sim_fslap.csv and sim_adtm.csv
(from data\\physics_test\\ailap.exe and adtm\\adtmlap.exe with a trace file argument). Prints the numbers and writes
data\\logs\\validation.md. Nothing is written outside data\\logs.
"""
import csv
import pathlib
import subprocess

import numpy as np

import asc_decode

ROOT = pathlib.Path(__file__).resolve().parents[2]
OUT = ROOT / "data" / "logs"
SVN = pathlib.Path(r"D:\SVN\logs\2026\02_Tau")
LOGS = {   # name in the report: file
    "Siemens 09-19, speed test": SVN / "2026-09-19_Siemens" / "3152-V573-b2cb_speed_test.asc",
    "Siemens 09-19, tc": SVN / "2026-09-19_Siemens" / "3153-V568-a73d_tc.asc",
    "Hoeffner 10-18, without TC": SVN / "2026-10-18_Höffner" / "3357-ohne TC.asc",
    "Hoeffner 10-18, TC works": SVN / "2026-10-18_Höffner" / "3368-TC funktioniert.asc",
    "Hoeffner 10-18, freestyle": SVN / "2026-10-18_Höffner" / "3354-freestyle.asc",
    "Hoeffner 10-18, skidpad": SVN / "2026-10-18_Höffner" / "3371-Skidpad.asc",
    "FSG endurance": SVN / "FSG" / "EnduFSG26.asc",
}
GEAR, RADIUS = 12.421, 0.205   # the ADTM's; the logs confirm them: lateral acceleration = yaw rate x this speed within 0.5 %
MOTOR_TO_MS = 2 * np.pi / 60 / GEAR * RADIUS
DT = 0.01
BANDS = [(30, 40), (40, 50), (50, 60), (60, 70), (70, 80), (80, 100)]   # km/h


class Run:
    """One log on a 100 Hz grid. Motors 1, 2 are the rear and 3, 4 the front (the fronts spin up in a launch)."""

    def __init__(self, name, path):
        npz = OUT / (path.stem + ".npz")
        if not npz.exists():
            asc_decode.decode(path, npz)
        d = np.load(npz)
        self.name = name
        self.t = t = np.arange(d["VCU_IMU_Accel.t"][0], d["VCU_IMU_Accel.t"][-1], DT)
        on = lambda message, signal: np.interp(t, d[f"{message}.t"], d[f"{message}.{signal}"])
        smooth = lambda x: np.convolve(x, np.ones(10) / 10, "same")   # 0.1 s: the IMU is noisy at 200 Hz
        self.rpm = np.array([on(f"Ephorus_inverter{n}outboundB", "Ephorus_SpeedActual") for n in "1234"])
        self.torque = np.array([on(f"Ephorus_inverter{n}outboundA", "Ephorus_TorqueActual") for n in "1234"])
        # an axle's mean cancels the inner and outer wheel in a corner; driven wheels spin up and braked ones lock,
        # so the slower axle counts under power and the faster one otherwise
        rear, front = self.rpm[:2].mean(axis=0), self.rpm[2:].mean(axis=0)
        self.v = np.where(self.torque.sum(axis=0) > 1, np.minimum(rear, front), np.maximum(rear, front)) * MOTOR_TO_MS
        self.s = np.cumsum(self.v) * DT
        still = self.v < 0.05
        self.ax = smooth(on("VCU_IMU_Accel", "VCU_IMU_Accel_X")) - on("VCU_IMU_Accel", "VCU_IMU_Accel_X")[still].mean()   # the IMU is not level
        self.ay = smooth(on("VCU_IMU_Accel", "VCU_IMU_Accel_Y")) - on("VCU_IMU_Accel", "VCU_IMU_Accel_Y")[still].mean()
        self.yaw = smooth(on("VCU_IMU_Gyro", "VCU_IMU_Gyro_Z"))
        self.pedal = on("COMBINED_APPS", "APPS") / 1000
        self.brake = on("AD2CAN_F1_1", "BrakePressureFront")
        self.steer = on("AD2CAN_F1_1", "Steering")
        self.power = (self.torque * self.rpm).sum(axis=0) * 2 * np.pi / 60 / 1000   # kW at the four motor shafts
        self.electric = on("Ephorus_GeneralOutbound", "Ephorus_DClinkVoltage12Actual") * on("IVT_Msg_Result_I", "IVT_Result_I") / 1000

    def launches(self):
        """Standing starts held flat out to 60 km/h: (start time, 0-60, 0-90, 75 m, largest ax, front wheel spin)."""
        found = []
        i = 0
        while i < len(self.t) - 800:
            if self.v[i] < 0.1 and self.v[i + 1] >= 0.1:
                window = slice(i, i + 800)
                v, t = self.v[window], self.t[window] - self.t[i]
                to60 = np.argmax(v > 60 / 3.6)
                to15 = np.argmax(v > 15 / 3.6)   # drivers roll onto the pedal: flat out counts from 15 km/h
                if to60 and to60 < 400 and self.pedal[i + to15:i + to60].min() > 0.9:
                    flat = np.argmax(self.pedal[i + to15:i + 800] < 0.9) + to15   # samples until the driver lifts
                    to90 = np.argmax(v > 90 / 3.6)
                    s = np.cumsum(v) * DT
                    to75 = np.argmax(s > 75)
                    spin = (self.rpm[2:, i:i + to60].mean() / np.maximum(self.rpm[:2, i:i + to60].mean(), 1) - 1) * 100
                    found.append((self.t[i], t[to60], t[to90] if to90 and to90 < flat else np.nan, t[to75] if to75 and to75 < flat else np.nan,
                                  self.ax[i:i + to60].max(), spin))
                    i += 800
            i += 1
        return found


def simulated(arguments):
    rows = list(csv.DictReader(subprocess.run([str(OUT / "sim_accel.exe"), *arguments], capture_output=True, text=True, check=True).stdout.splitlines()))
    result = {}
    for model in ("fslap", "adtm"):
        t, v, s, ax = np.array([[float(r[k]) for k in ("t", "v", "s", "ax")] for r in rows if r["run"] == model]).T
        go = t <= 7   # flat out for 7 s, then the brakes
        at = lambda kmh: np.interp(kmh / 3.6, v[go], t[go]) if v[go].max() > kmh / 3.6 else np.nan
        braking = ~go & (v > 40 / 3.6)
        # the circles: the grip at a speed is the most any steering angle reaches there
        circles = {}
        for r in rows:
            if r["run"].startswith(model + "_circle"):
                circles.setdefault(r["run"], []).append((float(r["v"]) * 3.6, abs(float(r["ay"]))))
        circles = [np.array(c) for c in circles.values()]
        grip = [max(c[(c[:, 0] >= a) & (c[:, 0] < b), 1].mean() for c in circles if ((c[:, 0] >= a) & (c[:, 0] < b)).any()) for a, b in BANDS]
        lap = next(float(r["t"]) for r in rows if r["run"] == model + "_lap")
        result[model] = dict(lap=lap, t60=at(60), t90=at(90), t75=np.interp(75, s[go], t[go]), ax=ax[go].max(), top=v.max() * 3.6,
                             brake=(-np.mean(ax[braking]), -ax[braking].min()), grip=grip)
    return result


def lap_trace(name):
    """The AI's flying lap from a physics test trace: speed, ax, ay (the columns of ailap.cpp and adtmlap.cpp)."""
    trace = np.loadtxt(OUT / name, delimiter=",")
    wraps = np.flatnonzero(np.diff(trace[:, 6]) < -500)
    lap = trace[wraps[0] + 1:wraps[1] + 1]
    return lap[-1, 0] - lap[0, 0], lap[:, 3], lap[:, 4], lap[:, 5]


def main():
    lines = []
    say = lambda text="": (print(text), lines.append(text))
    runs = {name: Run(name, path) for name, path in LOGS.items()}

    say("## Standing starts (flat out from rest)\n")
    say("| Run | start (s) | 0-60 km/h (s) | 0-90 km/h (s) | 75 m (s) | largest ax (m/s2) | front wheels faster than rear (%) |")
    say("|---|---|---|---|---|---|---|")
    for name, run in runs.items():
        for start, t60, t90, t75, ax, spin in run.launches():
            say(f"| {name} | {start:.1f} | {t60:.2f} | {t90:.2f} | {t75:.2f} | {ax:.1f} | {spin:.0f} |")
    models = {"fslap": "FS Lap model", "adtm": "ADTM"}
    stock = simulated([])
    for label, sim in (("stock (80 kW, 21 Nm)", stock), ("as logged (66 kW, 18.9 Nm)", simulated(["66", "18.9"]))):
        for model, r in sim.items():
            say(f"| **Simulator, {models[model]}, {label}** | | {r['t60']:.2f} | {r['t90']:.2f} | {r['t75']:.2f} | {r['ax']:.1f} | |")

    say("\n## Speed from the motors against the IMU\n")
    say("In a corner, lateral acceleration = yaw rate x speed. The slope of that line checks the gear ratio and tyre radius used for the speed (12.421, 0.205 m).\n")
    say("| Run | ay / (yaw rate x speed) | correlation |")
    say("|---|---|---|")
    for name, run in runs.items():
        cornering = (run.v > 5) & (np.abs(run.yaw) > 0.3)
        if cornering.sum() > 2000:
            centripetal = (run.yaw * run.v)[cornering]
            say(f"| {name} | {np.polyfit(centripetal, run.ay[cornering], 1)[0]:.3f} | {np.corrcoef(centripetal, run.ay[cornering])[0, 1]:.3f} |")

    say("\n## Powertrain at full throttle\n")
    say("| Run | largest motor torque (Nm) | motor shaft power, 99.9 % (kW) | electrical power, 99.9 % (kW) | top speed (km/h) |")
    say("|---|---|---|---|---|")
    for name, run in runs.items():
        say(f"| {name} | {np.percentile(run.torque, 99.99):.1f} | {np.percentile(run.power, 99.9):.1f} | {np.percentile(run.electric, 99.9):.1f} | {run.v.max() * 3.6:.1f} |")
    say("| **Simulator, stock** | 21.0 | 80 at the limit (77.6 at the wheels) | | 122.6 (FS Lap model), 118.5 (ADTM) |")

    say("\n## Limits: braking and cornering\n")
    say("Above 30 km/h, from the IMU (0.1 s average). The percentiles are of the moving time, so a log that is mostly straight-line running has small cornering numbers.\n")
    say("| Run | braking, 99 % and largest (m/s2) | lateral, 99 % and largest (m/s2) | combined, 99 % (m/s2) |")
    say("|---|---|---|---|")
    for name, run in runs.items():
        fast = run.v > 30 / 3.6
        if fast.sum() < 500:
            continue
        combined = np.hypot(run.ax, run.ay)[fast]
        say(f"| {name} | {np.percentile(-run.ax[fast], 99):.1f}, {(-run.ax[fast]).max():.1f} | {np.percentile(np.abs(run.ay[fast]), 99):.1f}, {np.abs(run.ay[fast]).max():.1f} | {np.percentile(combined, 99):.1f} |")
    for label, name in (("FS Lap model", "sim_fslap.csv"), ("ADTM", "sim_adtm.csv")):
        lap, v, ax, ay = lap_trace(name)
        fast = v > 30 / 3.6
        say(f"| **Simulator AI lap, {label}** ({lap:.2f} s) | {np.percentile(-ax[fast], 99):.1f}, {(-ax[fast]).max():.1f} | {np.percentile(np.abs(ay[fast]), 99):.1f}, {np.abs(ay[fast]).max():.1f} | {np.percentile(np.hypot(ax, ay)[fast], 99):.1f} |")
    for model, r in stock.items():
        say(f"| **Simulator, {models[model]}, full braking from top speed** | {r['brake'][0]:.1f} (mean), {r['brake'][1]:.1f} | | |")

    say("\n## Grip against speed: lateral acceleration in each speed band\n")
    say("Logs: 99 % of |ay| in the band, m/s2, which is at most what the driver used; blank where the log has under 3 s there. "
        "Simulator: the grip limit, from circles at fixed steering angles while the speed rises (sim_accel.cpp).\n")
    say("| Run | " + " | ".join(f"{a}-{b} km/h" for a, b in BANDS) + " |")
    say("|---|" + "---|" * len(BANDS))
    for name, run in runs.items():
        cells = []
        for a, b in BANDS:
            band = (run.v >= a / 3.6) & (run.v < b / 3.6)
            cells.append(f"{np.percentile(np.abs(run.ay[band]), 99):.1f}" if band.sum() * DT > 3 else "")
        say(f"| {name} | " + " | ".join(cells) + " |")
    for model, r in stock.items():
        say(f"| **Simulator grip limit, {models[model]}** | " + " | ".join(f"{g:.1f}" for g in r["grip"]) + " |")

    endurance = runs["FSG endurance"]
    # the stint: the longest stretch of driving (before it the car is pushed and turned around)
    edges = np.flatnonzero(np.diff(np.r_[0, (endurance.v > 1).astype(int), 0]))
    a, b = max(zip(edges[::2], edges[1::2]), key=lambda e: e[1] - e[0])
    t, v, s = endurance.t[a:b], endurance.v[a:b], endurance.s[a:b] - endurance.s[a]
    heading = np.abs(np.cumsum(endurance.yaw[a:b] * DT)) / (2 * np.pi)
    say("\n## FSG endurance log\n")
    say(f"- The stint: {t[-1] - t[0]:.0f} s, {s[-1]:.0f} m, {heading[-1]:.1f} turns of heading (a lap is one turn).")
    crossings = [np.argmax(heading >= k) for k in range(1, int(heading.max()) + 1)]
    laps, lengths = np.diff(t[crossings]), np.diff(s[crossings])
    say(f"- Laps (from one full turn of heading to the next): {', '.join(f'{x:.1f} s' for x in laps)}; lap length {', '.join(f'{x:.0f} m' for x in lengths)} "
        "(the motors' speed reads a few percent high under power, so the true length is a little shorter).")
    say(f"- Mean lap speed {np.mean(lengths / laps) * 3.6:.1f} km/h, top {v.max() * 3.6:.1f} km/h; largest motor torque {np.percentile(endurance.torque, 99.99):.1f} Nm, shaft power 99.9 % {np.percentile(endurance.power, 99.9):.1f} kW.")
    track = 1103.396   # the simulator's FSG track (m)
    say("\n| Simulator AI lap of its FSG track (1103 m) | lap (s) | mean speed (km/h) |")
    say("|---|---|---|")
    for label, sim in (("stock (80 kW, 21 Nm)", stock), ("endurance power (32 kW, 10.4 Nm)", simulated(["32", "10.4"]))):
        for model, r in sim.items():
            say(f"| {models[model]}, {label} | {r['lap']:.2f} | {track / r['lap'] * 3.6:.1f} |")
    (OUT / "validation.md").write_text("\n".join(lines) + "\n", encoding="utf-8")


if __name__ == "__main__":
    main()
