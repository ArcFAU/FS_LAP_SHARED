"""FS Czech (Most) for FS Lap from the official Czech map data, with the owner's drawing for the line.

    python tools\\logs\\track_from_log.py fscz-trace <picture>   the drawing's red line, placed on the Czech orthophoto
                                                              -> vehicle-data\\tracks\\fscz_drawing.json
    python tools\\logs\\track_from_log.py fscz                   -> vehicle-data\\tracks\\fscz.json

The line: the owner's red line on a satellite picture of the Autodrom Most polygon (web mercator, north up). Its centre
is traced and the picture is matched to the orthophoto of the Czech survey office (CUZK, Ortofoto CR) by correlating
the edges of both, over a scale and a shift; the line then has real coordinates and its true length. The picture
itself is not kept (map data under its service's terms); the traced line with its coordinates is.

The heights: CUZK's terrain model DMR 5G (1 m grid, about 0.2 m accurate; open data, CC BY 4.0, (c) CUZK), along the
line and as a 1 m grid around it, for the ground and both vehicle models. Heights are given above the track's lowest
point ("datum" holds that point's height above sea level).

The reference lap: Sigma's (the 2025 car, rear-wheel drive) fastest lap from FSCZ2025Endurance.csv, placed on the line
by the shape of its dead-reckoned lap. Sigma's distances come out 12 % short against the real line (its effective wheel
radius from this IMU is too small), so its lap is mapped by its share of the lap, not by metres.
"""
import io
import json
import urllib.request

import numpy as np

import track_from_log as T

ORTHO = "https://ags.cuzk.cz/arcgis1/rest/services/ORTOFOTO/MapServer/export"
DEM = "https://ags.cuzk.cz/arcgis2/rest/services/dmr5g/ImageServer/exportImage"
POLYGON = (13.5925, 50.5153, 13.6040, 50.5208)   # lon/lat box around the Autodrom Most polygon (the drawing's area and some margin)
MPP = 0.4      # web-mercator metres per pixel of the orthophoto used for the match
STEP = 1.0     # m, the terrain grid
MARGIN = 60.0  # m of terrain around the track


def fetch(url, params):
    query = "&".join(f"{k}={v}" for k, v in params.items())
    with urllib.request.urlopen(f"{url}?{query}", timeout=180) as r:
        return r.read()


def ortho_mercator():
    """the CUZK orthophoto of the polygon in web mercator: (greyscale array, its box)"""
    from PIL import Image
    from pyproj import Transformer
    to = Transformer.from_crs(4326, 3857, always_xy=True)
    x0, y0 = to.transform(POLYGON[0], POLYGON[1])
    x1, y1 = to.transform(POLYGON[2], POLYGON[3])
    w, h = round((x1 - x0) / MPP), round((y1 - y0) / MPP)
    data = fetch(ORTHO, {"bbox": f"{x0:.2f},{y0:.2f},{x1:.2f},{y1:.2f}", "bboxSR": 3857, "imageSR": 3857, "size": f"{w},{h}", "format": "jpg", "f": "image"})
    return np.asarray(Image.open(io.BytesIO(data)).convert("L")).astype(float), (x0, y0, x1, y1)


def register(picture):
    """the scale and shift that put the owner's picture on the orthophoto (both web mercator, north up)"""
    from PIL import Image
    from scipy import ndimage as nd
    from scipy.signal import fftconvolve
    rgb = np.asarray(Image.open(picture).convert("RGB")).astype(float)
    red = nd.binary_dilation((rgb[..., 0] > 110) & (rgb[..., 0] - rgb[..., 1] > 45) & (rgb[..., 0] - rgb[..., 2] > 40), iterations=6)   # the drawing, not the ground
    grey = rgb.mean(2)
    edges = lambda g: (lambda s: np.hypot(nd.sobel(s, 0), nd.sobel(s, 1)))(nd.gaussian_filter(g, 1.0))
    ortho, box = ortho_mercator()
    eo = edges(ortho)
    eo = (eo - eo.mean()) / eo.std()
    best = None
    for coarse in (np.arange(1.0, 3.0, 0.02), None):
        scales = coarse if coarse is not None else np.arange(best[1] - 0.02, best[1] + 0.021, 0.005)
        for s in scales:
            size = (round(grey.shape[1] * s), round(grey.shape[0] * s))
            if size[0] >= ortho.shape[1] or size[1] >= ortho.shape[0]:
                continue
            g = np.asarray(Image.fromarray(grey.astype(np.uint8)).resize(size, Image.BICUBIC)).astype(float)
            m = np.asarray(Image.fromarray((~red).astype(np.uint8) * 255).resize(size, Image.NEAREST)) > 0
            e = edges(g)
            e = np.where(m, (e - e[m].mean()) / e[m].std(), 0)
            c = fftconvolve(eo, e[::-1, ::-1], mode="valid")
            i = np.unravel_index(np.argmax(c), c.shape)
            score = c[i] / m.sum()
            if best is None or score > best[0]:
                best = (score, float(s), int(i[0]), int(i[1]))
    score, s, row, col = best
    print(f"the picture on the orthophoto: x{s:.3f}, at row {row} col {col}; match {score:.3f}")
    assert score > 0.25, "the picture does not match the orthophoto"
    return s, row, col, box, score


def trace(picture):
    """fscz-trace: the drawing's centreline (pixels, from the 'start') and its real coordinates"""
    T.trace_drawing(picture)   # writes the pixels
    out = T.TRACKS / "fscz_drawing.json"
    data = json.loads(out.read_text(encoding="utf-8"))
    px = np.array(data["px"])
    s, row, col, (x0, y0, x1, y1), score = register(picture)
    from pyproj import Transformer
    lon, lat = Transformer.from_crs(3857, 4326, always_xy=True).transform(x0 + (col + px[:, 0] * s) * MPP, y1 - (row + px[:, 1] * s) * MPP)
    data["lonlat"] = [[round(float(a), 7), round(float(b), 7)] for a, b in zip(lon, lat)]
    data["georeference"] = {"orthophoto": "CUZK Ortofoto CR (ags.cuzk.cz ORTOFOTO MapServer), web mercator", "box3857": [x0, y0, x1, y1],
                            "metresPerPixel3857": MPP, "scale": s, "row": row, "col": col, "match": round(float(score), 3)}
    out.write_text(json.dumps(data), encoding="utf-8")
    print(f"georeferenced {len(px)} points -> {out}")


def dem(lonlat_box):
    """DMR 5G heights over a lon/lat box: (array, S-JTSK box), 0.5 m grid"""
    from PIL import Image
    from pyproj import Transformer
    to = Transformer.from_crs(4326, 5514, always_xy=True)
    xs, ys = zip(*(to.transform(a, b) for a in lonlat_box[0::2] for b in lonlat_box[1::2]))
    x0, x1, y0, y1 = np.floor(min(xs)) - 5, np.ceil(max(xs)) + 5, np.floor(min(ys)) - 5, np.ceil(max(ys)) + 5
    w, h = int((x1 - x0) / 0.5), int((y1 - y0) / 0.5)
    data = fetch(DEM, {"bbox": f"{x0},{y0},{x1},{y1}", "bboxSR": 5514, "imageSR": 5514, "size": f"{w},{h}", "format": "tiff",
                       "pixelType": "F32", "interpolation": "RSP_BilinearInterpolation", "f": "image"})
    z = np.asarray(Image.open(io.BytesIO(data)), dtype=float)
    assert z.shape == (h, w) and np.all((z > 100) & (z < 1700)), "DMR 5G gave no heights there"
    return z, (x0, y0, x1, y1)


def build():
    """fscz: the track in true metres (north up), its heights and terrain, Sigma's lap as the reference"""
    from pyproj import Transformer
    from scipy import ndimage as nd
    drawing = json.loads((T.TRACKS / "fscz_drawing.json").read_text(encoding="utf-8"))
    lonlat = np.array(drawing["lonlat"])
    lon0, lat0 = lonlat.mean(0)
    local = f"+proj=aeqd +lat_0={lat0} +lon_0={lon0} +datum=WGS84 +units=m"   # true metres around the track
    e, n = Transformer.from_crs(4326, local, always_xy=True).transform(lonlat[:, 0], lonlat[:, 1])
    # the game's frame: x east, y south (y down), the drawing's pixel noise smoothed over about 2.5 m
    x, y = T.smooth_loop(np.asarray(e), 5), T.smooth_loop(-np.asarray(n), 5)
    xx, yy, length = T.resample(x, y)
    # the terrain: a 1 m grid in the game's frame over the track and MARGIN around it, from DMR 5G
    gx = np.arange(np.floor(xx.min() - MARGIN), np.ceil(xx.max() + MARGIN) + STEP, STEP)
    gy = np.arange(np.floor(yy.min() - MARGIN), np.ceil(yy.max() + MARGIN) + STEP, STEP)
    GX, GY = np.meshgrid(gx, gy)   # rows: y
    to_ll = Transformer.from_crs(local, 4326, always_xy=True)
    glon, glat = to_ll.transform(GX, -GY)
    z, (kx0, ky0, kx1, ky1) = dem((glon.min(), glat.min(), glon.max(), glat.max()))
    to_k = Transformer.from_crs(4326, 5514, always_xy=True)
    kx, ky = to_k.transform(glon, glat)
    G = nd.map_coordinates(z, [(ky1 - ky) / 0.5, (kx - kx0) / 0.5], order=1)
    cx, cy = (xx - gx[0]) / STEP, (yy - gy[0]) / STEP
    zc = nd.map_coordinates(G, [cy, cx], order=1)
    datum = float(zc.min())
    print(f"FS Czech line {length:.1f} m; heights {zc.min():.2f} .. {zc.max():.2f} m above sea level (rise {np.ptp(zc):.2f} m); "
          f"terrain {len(gx)} x {len(gy)} at {STEP} m, {G.min():.1f} .. {G.max():.1f} m")
    grade = np.gradient(zc) / 1.0
    print(f"  steepest along the line: up {grade.max() * 100:+.1f} %, down {grade.min() * 100:+.1f} %")
    # the reference: Sigma's fastest of its last laps, fitted onto the line by shape (rotation, shift, free scale)
    laps, radius = T.sigma_laps()
    n_pts = len(xx)
    B = np.c_[xx, yy]
    fits = []
    for lap in laps[-3:]:
        psi = np.cumsum(lap["r"]) * 0.01
        lx, ly = np.cumsum(lap["v"] * np.cos(psi)) * 0.01, -np.cumsum(lap["v"] * np.sin(psi)) * 0.01
        w = np.arange(1, len(lx) + 1) / len(lx)
        ax, ay, _ = T.resample(lx - w * lx[-1], ly - w * ly[-1])
        idx = np.linspace(0, len(ax), n_pts, endpoint=False).astype(int)
        A0 = np.c_[ax[idx], ay[idx]]
        best = None
        for sh in range(0, n_pts, 2):
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
        print(f"  Sigma's lap of {lap['t']:.1f} s on the line: {best[0]:.1f} m apart (rms) once scaled x{best[2]:.3f}")
    assert max(f[0] for f in fits) < 8, "Sigma's laps do not lie on the line"
    rms, sh, sc, fast = min(fits, key=lambda f: f[3]["t"])
    s = np.cumsum(fast["v"]) * 0.01
    t_lap = np.arange(1, len(s) + 1) * 0.01
    s_at = (np.arange(n_pts) / n_pts * s[-1] + sh / n_pts * s[-1]) % s[-1]
    t0 = np.interp(sh / n_pts * s[-1], s, t_lap)
    t_ref = (np.interp(s_at, s, t_lap) - t0) % fast["t"]
    v_ref = np.interp(s_at, s, fast["v"]) * length / s[-1]   # its speed on the real line's distance
    ox, oy = 40 - xx.min(), 40 - yy.min()
    T.write("FS Czech", T.TRACKS / "fscz.json", xx + ox, yy + oy, t_ref, v_ref, fast["t"],
            f"{T.SIGMA_CSV.name} (Sigma, rear-wheel drive), its fastest of the last laps", "Sigma 2025",
            {"shape": "the owner's drawing on the CUZK orthophoto (fscz_drawing.json)", "heights": "CUZK DMR 5G (CC BY 4.0, (c) CUZK)",
             "georeferenceMatch": drawing["georeference"]["match"], "sigmaOnLine_rms_m": [round(float(f[0]), 1) for f in fits],
             "sigmaScale": [round(float(f[2]), 3) for f in fits], "riseAlongLine_m": round(float(np.ptp(zc)), 2)},
            {"z": [round(float(v - datum), 3) for v in zc], "datum_m": round(datum, 3),
             "terrain": {"x0": float(gx[0] + ox), "y0": float(gy[0] + oy), "step": STEP, "nx": len(gx), "ny": len(gy),
                         "z": [round(float(v - datum), 2) for v in G.ravel()]}})


if __name__ == "__main__":
    import sys
    trace(sys.argv[1]) if len(sys.argv) > 1 else build()
