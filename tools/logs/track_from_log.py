"""Event tracks for FS Lap from the cars' logs: the line the car drove, as a closed track with its best lap as the reference.

    python tools\\logs\\track_from_log.py fsf           FS France 2026 (3081-Endu.asc, Tau)  -> vehicle-data\\tracks\\fsf.json
    python tools\\logs\\track_from_log.py fscz          FS Czech from the Czech map data (track_fscz.py) -> fscz.json
    python tools\\logs\\track_from_log.py fscz-trace <image>   the drawing's red line, placed on the Czech orthophoto -> fscz_drawing.json
    python tools\\logs\\track_from_log.py fsg           FSG 2026 from EnduFSG26.asc: only a check of the LTS track
                                                         (data\\logs\\fsg2026_log_track.json; the game uses the LTS track)
Then tools\\track_to_ue.py, the import (FSLAP_IMPORT=tracks) and a build.

From a Tau log (fsf, fsg): no GNSS in the logs, so the line is dead reckoning: heading from the IMU's yaw rate, distance
from the motor speeds, scaled by the log's own check (lateral acceleration = yaw rate x speed), separately under power
and off it (wheel slip). The lap period comes from the curvature's autocorrelation; laps are cut in the middle of the
longest straight and found again by the curvature's shape; the lap whose ends meet best is the line, closed with a small
correction. The start line is where the car stood when the stint began, and the reference is the car's fastest flying
lap from that line. Checks: the bends agree with the logged steering, no two sections of road overlap.

FS Czech (track_fscz.py): the owner's drawing placed on the Czech survey office's orthophoto (true scale and position)
and its heights from their terrain model DMR 5G; Sigma's log (the 2025 car, in the team ADTM's inputs) gives only the
reference lap: its gyro drifts (0.82 to 1.04 turns per lap) and its distances come out 12 % short, so it cannot make
the track itself.

What a track is not: the centreline between the cones. The road width (4 m) and the cones are FS Lap's.
"""
import json
import pathlib
import sys

import numpy as np

import validate as V

ROOT = pathlib.Path(__file__).resolve().parents[2]
TRACKS = ROOT / "vehicle-data" / "tracks"
DS = 0.5   # m, the curvature grid
SVN = pathlib.Path(r"D:\SVN\logs\2026\02_Tau")
SIGMA_CSV = pathlib.Path(r"D:\VD\advanced-dual-track-model\MATLAB\input\FSCZ2025Endurance.csv")


def smooth_loop(a, n):
    return np.convolve(np.r_[a[-n:], a, a[:n]], np.ones(n) / n, "same")[n:-n]


def resample(x, y):
    """closed polyline -> 1 m points, its length"""
    seg = np.hypot(np.diff(np.r_[x, x[0]]), np.diff(np.r_[y, y[0]]))
    cum = np.r_[0, np.cumsum(seg)]
    length = cum[-1]
    su = np.linspace(0, length, int(round(length)), endpoint=False)
    return np.interp(su, cum, np.r_[x, x[0]]), np.interp(su, cum, np.r_[y, y[0]]), length


def game_curvature(x, y, h=3):
    """as the game's core computes it: + is a right turn (the game's frame has y down)"""
    a1 = np.arctan2(y - np.roll(y, h), x - np.roll(x, h))
    a2 = np.arctan2(np.roll(y, -h) - y, np.roll(x, -h) - x)
    return np.angle(np.exp(1j * (a2 - a1))) / h


def closest_sections(x, y):
    step, n = 2, len(x)
    idx = np.arange(0, n, step)
    return min(np.hypot(x[idx] - x[i], y[idx] - y[i])[np.minimum(abs(idx - i), n - abs(idx - i)) > 30].min() for i in idx)


def orient(x, y):
    """the longest straight along +x, the track's corner at (40, 40)"""
    k = np.abs(game_curvature(x, y))
    low = np.r_[k < 0.012, k < 0.012]
    e = np.flatnonzero(np.diff(np.r_[0, low.astype(int), 0]))
    a, b = max(zip(e[::2], e[1::2]), key=lambda p: p[1] - p[0])
    i, j = a % len(x), (b - 1) % len(x)
    ang = np.arctan2(y[j] - y[i], x[j] - x[i])
    x, y = x * np.cos(-ang) - y * np.sin(-ang), x * np.sin(-ang) + y * np.cos(-ang)
    return x - x.min() + 40, y - y.min() + 40


def write(name, out, xx, yy, ref_t, ref_v, lap_time, source, ref_name, made, extra=None):
    length = len(xx)
    ls = np.linspace(0, length, len(ref_t), endpoint=False)
    grid = np.arange(0, length, 2.0)
    sep = closest_sections(xx, yy)
    assert sep > 6, "two sections of the road would overlap"
    data = {
        "name": name, "x": [round(float(v), 2) for v in xx], "y": [round(float(v), 2) for v in yy],
        "ref": {"s": [round(float(v), 1) for v in grid], "t": [round(float(v), 3) for v in np.interp(grid, ls, ref_t)],
                "v": [round(float(v), 2) for v in np.interp(grid, ls, ref_v)], "length": float(length),
                "lapTime": round(float(lap_time), 3), "splits": [round(float(np.interp(length * f, ls, ref_t)), 3) for f in (1 / 3, 2 / 3)],
                "source": source, "name": ref_name},
        "made": dict(made, by="tools/logs/track_from_log.py", closestSections_m=round(float(sep), 1)),
    }
    data.update(extra or {})   # FS Czech: the centreline's heights and the terrain around it
    out.parent.mkdir(parents=True, exist_ok=True)
    out.write_text(json.dumps(data, separators=(",", ":")), encoding="utf-8")
    print(f"{name}: {length} m, closest sections {sep:.1f} m, reference {ref_name} {lap_time:.2f} s -> {out}")


def from_tau_log(name, log, out, ref_name):
    r = V.Run(name, log)
    edges = np.flatnonzero(np.diff(np.r_[0, (r.v > 1).astype(int), 0]))
    a, b = max(zip(edges[::2], edges[1::2]), key=lambda e: e[1] - e[0])   # the stint: the longest stretch of driving
    drive = r.torque.sum(axis=0) > 5
    corner = (r.v > 5) & (np.abs(r.yaw) > 0.3)
    corner[:a] = corner[b:] = False
    fit = lambda m: np.sum(r.ay[m] * r.yaw[m] * r.v[m]) / np.sum((r.yaw[m] * r.v[m]) ** 2)
    k_drive, k_coast = fit(corner & drive), fit(corner & ~drive)
    v = r.v[a:b] * np.where(drive[a:b], k_drive, k_coast)
    s = np.cumsum(v) * V.DT
    g = np.arange(0, s[-1], DS)
    kappa = np.convolve(np.gradient(np.interp(g, s, np.cumsum(r.yaw[a:b]) * V.DT), DS), np.ones(9) / 9, "same")
    t, speed, steer = np.interp(g, s, r.t[a:b]), np.interp(g, s, v), np.interp(g, s, r.steer[a:b])
    # the lap: the curvature's period
    kk = kappa - kappa.mean()
    lags = np.arange(int(500 / DS), int(1700 / DS))
    ac = [np.dot(kk[:-L], kk[L:]) / np.sqrt(np.dot(kk[:-L], kk[:-L]) * np.dot(kk[L:], kk[L:])) for L in lags]
    n = int(lags[int(np.argmax(ac))])
    print(f"{name}: speed factors {k_drive:.3f} / {k_coast:.3f}, lap period {n * DS:.1f} m (autocorrelation {max(ac):.3f})")
    # lap marks: the middle of the longest straight of the second lap, found again on the others by the curvature around it
    low = np.abs(kappa[n:2 * n]) < 0.012
    e = np.flatnonzero(np.diff(np.r_[0, low.astype(int), 0]))
    sa, sb = max(zip(e[::2], e[1::2]), key=lambda x: x[1] - x[0])
    mid = n + (sa + sb) // 2
    ref = kappa[mid - 400:mid + 400]
    marks = []
    for j in range(-3, 6):
        tries = [c for c in range(mid + j * n - 100, mid + j * n + 100) if c - 400 >= 0 and c + 400 <= len(kappa)]
        if tries:
            marks.append(max(tries, key=lambda c: np.corrcoef(kappa[c - 400:c + 400], ref)[0, 1]))
    laps = []
    for p, q in zip(marks[:-1], marks[1:]):
        turn = np.sum(kappa[p:q]) * DS / (2 * np.pi)
        # the ends after the heading is made exactly one turn (FSF's gyro reads about 2 % low; FSG's about 1 %)
        psi = np.cumsum(kappa[p:q] - (turn - np.sign(turn)) * 2 * np.pi / ((q - p) * DS)) * DS
        gap = np.hypot(np.sum(np.cos(psi)) * DS, np.sum(np.sin(psi)) * DS)
        laps.append((gap + 50 * abs(abs(turn) - 1), p, q, gap, turn))
        print(f"  lap {p * DS:5.0f}-{q * DS:5.0f} m: {(q - p) * DS:.1f} m, {t[q] - t[p]:.2f} s, heading {turn:+.4f} turns, ends {gap:.1f} m apart")
    _, p, q, gap, turn = min(laps)
    assert gap < 8 and abs(abs(turn) - 1) < 0.03, "no lap closes well enough"
    k = kappa[p:q] - (np.sum(kappa[p:q]) * DS - 2 * np.pi * np.sign(turn)) / ((q - p) * DS)   # exactly one turn
    psi = np.cumsum(k) * DS
    x, y = np.cumsum(np.cos(psi)) * DS, np.cumsum(np.sin(psi)) * DS
    w = np.arange(1, len(k) + 1) / len(k)
    x, y = x - w * x[-1], y - w * y[-1]   # the ends meet
    xx, yy, length = resample(np.r_[0, x[:-1]], -np.r_[0, y[:-1]])   # into the game's frame: y down
    agree = np.corrcoef(game_curvature(xx, yy), np.interp(np.arange(len(xx)), np.linspace(0, len(xx), q - p, endpoint=False), steer[p:q]))[0, 1]
    print(f"  line: lap {p * DS:.0f}-{q * DS:.0f} m, {length:.1f} m, bends against the logged steering r = {agree:.3f}")
    assert agree > 0.8, "the track turns the other way from the driver's steering"
    # the start line: the stint's first metre (the endurance starts there); the laps from it, the fastest flying one
    start = int(round((-p * DS) % length))
    xx, yy = orient(np.roll(xx, -start), np.roll(yy, -start))
    lap_n = q - p
    a0 = (p + int(round(start / DS))) % lap_n
    a0 = 0 if a0 > lap_n - 4 else a0
    runs = [(c, c + lap_n) for c in range(a0, len(t) - lap_n, lap_n)]
    for c, d in runs:
        print(f"  from the line at {c * DS:5.0f} m: {t[d] - t[c]:.2f} s")
    c, d = min(runs[1:] if a0 == 0 and len(runs) > 1 else runs, key=lambda e: t[e[1]] - t[e[0]])   # not the standing start
    write(name, out, xx, yy, t[c:d] - t[c], speed[c:d], t[d] - t[c], f"{log.name}, the lap from the line at {c * DS:.0f} m of the stint", ref_name,
          {"speedFactor": [round(k_drive, 4), round(k_coast, 4)], "line": f"stint {p * DS:.0f}-{q * DS:.0f} m", "closingGap_m": round(float(gap), 2),
           "closingTurn": round(float(turn), 4), "steeringAgreement": round(float(agree), 3)})


def trace_drawing(image):
    """the owner's red line on the satellite picture -> its centreline in pixels, in driving order from the 'start'"""
    from PIL import Image
    from scipy import ndimage as nd
    from scipy.spatial import cKDTree
    im = np.asarray(Image.open(image)).astype(int)
    red = (im[..., 0] > 110) & (im[..., 0] - im[..., 1] > 45) & (im[..., 0] - im[..., 2] > 40)
    lab, n = nd.label(red, structure=np.ones((3, 3)))
    band = nd.binary_closing(lab == 1 + int(np.argmax(nd.sum(red, lab, range(1, n + 1)))), iterations=2)   # the line, not the handwriting
    inner = nd.binary_fill_holes(band) & ~band
    outside = ~nd.binary_fill_holes(band)
    mid = band & (np.abs(nd.distance_transform_edt(~inner) - nd.distance_transform_edt(~outside)) <= 1.0)
    pts = np.c_[np.nonzero(mid)[1], np.nonzero(mid)[0]].astype(float)
    tree = cKDTree(pts)
    cur = int(np.argmin(np.hypot(*(pts - START_PX).T)))
    order, used = [cur], np.zeros(len(pts), bool)
    used[cur] = True
    while True:
        dd, ii = tree.query(pts[cur], k=40)
        nxt = next((j for d, j in zip(dd, ii) if not used[j] and d < 8), None)
        if nxt is None:
            break
        for j in tree.query_ball_point(pts[nxt], 1.6):
            used[j] = True
        order.append(nxt)
        cur = nxt
    p = pts[order]
    assert np.hypot(*(p[-1] - p[0])) < 4, "the traced line does not close"
    if np.sum(p[:, 0] * np.roll(p[:, 1], -1) - np.roll(p[:, 0], -1) * p[:, 1]) > 0:   # anticlockwise on the picture, as the arrow points
        p = p[::-1]
    out = TRACKS / "fscz_drawing.json"
    out.parent.mkdir(parents=True, exist_ok=True)
    out.write_text(json.dumps({"note": "centreline of the owner's red line on a 695 x 441 satellite picture (pixels, y down), from the 'start' in driving direction",
                               "px": [[round(float(a), 1), round(float(b), 1)] for a, b in p]}), encoding="utf-8")
    print(f"traced {len(p)} points -> {out}")


START_PX = np.array([682.0, 190.0])   # the 'start' arrow on the drawing: the line on the right-hand straight


def sigma_laps():
    """Sigma's FS Czech 2025 endurance: laps cut by the steering's 70.7 s pattern; per lap the yaw rate's drift is taken out"""
    d = np.genfromtxt(SIGMA_CSV, delimiter=",", skip_header=1, usecols=range(1, 15))
    st, ay, r, w = d[:, 0], d[:, 7], d[:, 11], (d[:, 12] + d[:, 13]) / 2   # front wheels: undriven on Sigma
    m = (w > 20) & (np.abs(r) > 0.3)
    radius = np.sum(ay[m] * r[m] * w[m]) / np.sum((r[m] * w[m]) ** 2)   # the log's own check, lateral acceleration = yaw rate x speed
    v = w * radius
    period = 7070
    mid = len(st) // 2
    tpl = st[mid:mid + period] - st[mid:mid + period].mean()
    score = [(np.dot(st[i:i + period] - st[i:i + period].mean(), tpl) / (np.linalg.norm(st[i:i + period] - st[i:i + period].mean()) * np.linalg.norm(tpl) + 1e-9), i)
             for i in range(0, len(st) - period, 20)]
    starts = []
    for c, i in sorted(score, reverse=True):
        if c < 0.5:
            break
        if all(abs(i - j) > 0.7 * period for j in starts):
            starts.append(i)
    starts.sort()
    laps = []
    for a, b in zip(starts[:-1], starts[1:]):
        if b - a > 1.3 * period:
            continue   # a slow lap (the driver change)
        rr = r[a:b] - (np.sum(r[a:b]) * 0.01 - 2 * np.pi) / ((b - a) * 0.01)
        laps.append(dict(t=(b - a) * 0.01, dist=np.sum(v[a:b]) * 0.01, v=v[a:b], r=rr))
    return laps, radius


def from_drawing():
    trace = json.loads((TRACKS / "fscz_drawing.json").read_text(encoding="utf-8"))["px"]
    px = np.array(trace)
    x, y = smooth_loop(px[:, 0], 5), smooth_loop(px[:, 1], 5)
    laps, radius = sigma_laps()
    lengths = np.array([lap["dist"] for lap in laps])
    length = float(np.median(lengths))
    seg = np.hypot(np.diff(np.r_[x, x[0]]), np.diff(np.r_[y, y[0]])).sum()
    scale = length / seg
    xx, yy, L = resample(x * scale, y * scale)   # the picture's frame is the game's (y down), the start first
    n = len(xx)
    print(f"FS Czech: Sigma (radius {radius:.4f} m) laps {', '.join(f'{lap['t']:.1f} s / {lap['dist']:.0f} m' for lap in laps)}")
    # the check: Sigma's laps by dead reckoning (their own drift taken out), fitted onto the drawing by a rotation, a shift
    # and a free scale. The early laps drift unevenly within the lap, so the last three are used.
    B = np.c_[xx, yy]
    fits = []
    for lap in laps[-3:]:
        psi = np.cumsum(lap["r"]) * 0.01
        lx, ly = np.cumsum(lap["v"] * np.cos(psi)) * 0.01, -np.cumsum(lap["v"] * np.sin(psi)) * 0.01   # the game's frame: y down
        w = np.arange(1, len(lx) + 1) / len(lx)
        ax, ay_, _ = resample(lx - w * lx[-1], ly - w * ly[-1])
        idx = np.linspace(0, len(ax), n, endpoint=False).astype(int)
        A0 = np.c_[ax[idx], ay_[idx]]
        best = None
        for sh in range(0, n, 2):
            A = np.roll(A0, -sh, 0)
            am, bm = A.mean(0), B.mean(0)
            u, _, vt = np.linalg.svd((A - am).T @ (B - bm))
            R = (u @ vt).T
            if np.linalg.det(R) < 0:
                continue
            Ar = (A - am) @ R.T
            sc = np.sum(Ar * (B - bm)) / np.sum(Ar * Ar)
            rms = np.sqrt(np.mean(np.sum((Ar * sc + bm - B) ** 2, 1)))
            if best is None or rms < best[0]:
                best = (rms, sh, sc)
        fits.append((best[0], best[1], best[2], lap))
        print(f"  Sigma's lap of {lap['t']:.1f} s on the drawing: {best[0]:.1f} m apart (rms), its size x{best[2]:.3f} of the drawing's")
    assert max(f[0] for f in fits) < 8 and all(abs(f[2] - 1) < 0.03 for f in fits), "the drawing and Sigma's log disagree"
    # the reference: the fastest of those laps, from the drawing's start line
    rms, sh, sc, fast = min(fits, key=lambda f: f[3]["t"])
    s = np.cumsum(fast["v"]) * 0.01
    t_lap = np.arange(1, len(s) + 1) * 0.01
    s_at = (np.arange(n) / n * s[-1] + sh / n * s[-1]) % s[-1]   # the lap's distance at each drawing point
    t0 = np.interp(sh / n * s[-1], s, t_lap)
    t_ref = (np.interp(s_at, s, t_lap) - t0) % fast["t"]
    v_ref = np.interp(s_at, s, fast["v"])
    xx, yy = xx - xx.min() + 40, yy - yy.min() + 40
    write("FS Czech", TRACKS / "fscz.json", xx, yy, t_ref, v_ref, fast["t"], f"{SIGMA_CSV.name} (Sigma, rear-wheel drive), its fastest of the last laps", "Sigma 2025",
          {"lengthFrom": f"median of Sigma's {len(laps)} laps: {', '.join(f'{d:.0f}' for d in lengths)} m", "metresPerPixel": round(scale, 4),
           "sigmaOnDrawing_rms_m": [round(float(f[0]), 1) for f in fits], "sigmaScale": [round(float(f[2]), 3) for f in fits],
           "shape": "the owner's drawing (fscz_drawing.json)"})


def main():
    what = sys.argv[1] if len(sys.argv) > 1 else ""
    if what == "fsf":
        from_tau_log("FS France", SVN / "FSF" / "3081-Endu.asc", TRACKS / "fsf.json", "Tau 2026")
    elif what == "fsg":
        from_tau_log("FSG (log check)", SVN / "FSG" / "EnduFSG26.asc", ROOT / "data" / "logs" / "fsg2026_log_track.json", "Tau 2026")
    elif what == "fscz-trace":
        import track_fscz
        track_fscz.trace(sys.argv[2])
    elif what == "fscz":
        import track_fscz
        track_fscz.build()
    else:
        sys.exit(__doc__)


if __name__ == "__main__":
    main()
