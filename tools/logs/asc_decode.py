"""Decodes a Vector .asc CAN log from the car with the team's DBC into a NumPy file.

    python tools\\logs\\asc_decode.py <log.asc> [out.npz]

The log and the DBC are only read (the logs are an SVN checkout). The result goes to data\\logs\\<name>.npz, which git
ignores: for every message kept, "<Message>.t" (s) and "<Message>.<Signal>" arrays.
Only the vehicle dynamics messages below are kept.
"""
import pathlib
import re
import sys

import numpy as np

ROOT = pathlib.Path(__file__).resolve().parents[2]
DBCS = [pathlib.Path(r"D:\VD\Tools\homolog\config\database2023_merged.dbc"), pathlib.Path(r"D:\VD\Tools\homolog\config\VD_Signals.dbc")]
# IMU, front sensor node (steering, brake pressures, pedal, front wheel speeds), pedal, the four inverters, DC link, IVT
KEEP = [0x6C0, 0x6C1, 0x103, 0x105, 0x012, 0x383, 0x393, 0x3A3, 0x3B3, 0x385, 0x395, 0x3A5, 0x3B5, 0x400, 0x521, 0x522]

SIGNAL = re.compile(r'\s*SG_ (\w+) : (\d+)\|(\d+)@(\d)([+-]) \(([^,]+),([^)]+)\)')


def read_dbc(ids):
    """{id: (message name, [(signal, start bit, length, little-endian, signed, factor, offset)])}; the first DBC that has an id wins."""
    found = {}
    for path in DBCS:
        current = None
        for line in path.read_text(encoding="latin-1").splitlines():
            if line.startswith("BO_ "):
                number, name = int(line.split()[1]), line.split()[2].rstrip(":")
                current = number if number in ids and number not in found else None
                if current is not None:
                    found[current] = (name, [])
            elif current is not None:
                m = SIGNAL.match(line)
                if m:
                    found[current][1].append((m.group(1), int(m.group(2)), int(m.group(3)), m.group(4) == "1", m.group(5) == "-", float(m.group(6)), float(m.group(7))))
                elif not line.strip():
                    current = None
    return found


def decode(asc, out):
    dbc = read_dbc(set(KEEP))
    times = {i: [] for i in dbc}
    frames = {i: [] for i in dbc}
    with open(asc, "r", encoding="latin-1") as log:
        for line in log:
            part = line.split()
            # "   0.005719 1  103  Rx   d 8 58 0B ..."
            if len(part) < 7 or part[3] != "Rx" or part[4] != "d":
                continue
            try:
                number = int(part[2].rstrip("x"), 16)
            except ValueError:
                continue
            if number in dbc:
                times[number].append(float(part[0]))
                frames[number].append(bytes.fromhex("".join(part[6:6 + int(part[5])])).ljust(8, bytes(1)))
    result = {}
    for number, (name, signals) in dbc.items():
        data = b"".join(frames[number])
        little, big = np.frombuffer(data, dtype="<u8"), np.frombuffer(data, dtype=">u8")
        result[f"{name}.t"] = np.array(times[number])
        for signal, start, length, intel, signed, factor, offset in signals:
            # Motorola: the DBC's start bit is the most significant one, counted 7..0 in byte 0, 15..8 in byte 1, ...
            shift = start if intel else 64 - (start // 8 * 8 + 7 - start % 8) - length
            value = (((little if intel else big) >> np.uint64(shift)) & np.uint64((1 << length) - 1)).astype(np.int64)
            if signed:
                value = np.where(value >= 1 << (length - 1), value - (1 << length), value)
            result[f"{name}.{signal}"] = (value * factor + offset).astype(np.float32)
        print(f"{number:#05x} {name}: {len(little)} frames, {len(signals)} signals")
    out.parent.mkdir(parents=True, exist_ok=True)
    np.savez_compressed(out, **result)
    print(f"wrote {out}")


if __name__ == "__main__":
    source = pathlib.Path(sys.argv[1])
    decode(source, pathlib.Path(sys.argv[2]) if len(sys.argv) > 2 else ROOT / "data" / "logs" / (source.stem + ".npz"))
