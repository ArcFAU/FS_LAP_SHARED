# Builds the whole city of Erlangen for FS Lap's city mode from the open data that tools\erlangen_fetch.py downloads:
#   terrain    Bavarian DGM1 (1 m), averaged onto a 2 m grid: the ground the car drives on and the game draws
#   buildings  Bavarian LoD2 (their real walls and roofs); their ground outlines are the walls the car hits
#     both (c) Bayerische Vermessungsverwaltung, www.geodaten.bayern.de, CC BY 4.0
#   map        OpenStreetMap: roads, bridges, land use, water, trees, lamps; the city boundary is the edge of the world
#     (c) OpenStreetMap contributors, ODbL 1.0
# Writes
#   FSLapUE\Content\FS\Data\Erlangen.bin   what the physics and the game read at run time (HoMoLapCore.cpp, MakeCityTrack)
#   data\erlangen\ue\SM_Erl_B_<i>_<j>.glb  buildings of one 500 m tile, SM_Erl_R_<i>_<j>.glb its roads and bridges
#   data\erlangen\ue\materials.json, T_ErlWall.png, plan.png (a picture of the whole map to check by eye)
#   python tools\erlangen_to_ue.py [data]     data: only the run-time file, the meshes left as they are
import glob
import heapq
import json
import math
import os
import random
import re
import struct
import sys
import time

import numpy as np
import osmium
from PIL import Image, ImageDraw
from pyproj import Transformer
from scipy import ndimage
from scipy.spatial import cKDTree

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import glb
from city_to_ue import facade_texture

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
DATA = os.path.join(ROOT, 'data', 'erlangen')
OUT = os.path.join(DATA, 'ue')
BIN = os.path.join(ROOT, 'FSLapUE', 'Content', 'FS', 'Data', 'Erlangen.bin')
BOUNDARY = 62403           # OSM relation of the city of Erlangen (admin level 6, Regionalschluessel 095620000000)
STEP = 2.0                 # m between terrain samples
MARGIN = 600.0             # terrain beyond the boundary, so the world does not end at a cliff
TILE = 500.0               # mesh tiles
FOCUS = (49.5945, 11.0080)  # the timed circuit is looked for near here (east of the old town), as before
LAP, REACH = 3000, 1300
TREE_MARGIN = 300.0        # m round the boundary that still gets trees
MIN_RADIUS = 12.0          # m: the circuit's tightest corner
TO_UTM = Transformer.from_crs(4326, 25832, always_xy=True)
MESHES = 'data' not in sys.argv[1:]

WIDTH = {'motorway': 14, 'trunk': 12, 'primary': 11, 'secondary': 10, 'tertiary': 9, 'unclassified': 6.5, 'residential': 6.5,
         'living_street': 5.5, 'service': 4.0, 'motorway_link': 7, 'trunk_link': 7, 'primary_link': 7, 'secondary_link': 7, 'tertiary_link': 7}
MAIN = {'motorway', 'trunk', 'primary', 'secondary', 'motorway_link', 'trunk_link', 'primary_link', 'secondary_link'}
CIRCUIT_COST = {'trunk': 1.0, 'primary': 1.0, 'secondary': 1.0, 'tertiary': 1.3, 'trunk_link': 1.6, 'primary_link': 1.6,
                'secondary_link': 1.6, 'tertiary_link': 1.8, 'unclassified': 2.2, 'residential': 2.6}
PATHS = {'footway': 2.0, 'cycleway': 2.0, 'pedestrian': 4.0, 'steps': 2.0, 'path': 1.5, 'track': 3.0, 'bridleway': 1.5}

# the ground's look by land use: linear colour, wetness (0 dry .. 1 water: the game's roughness), how far the game sinks the
# ground under the road meshes (so the drawn road is the surface the car drives on, no ground poking through) and its grip
LAND = [  # name, colour, wet, sink, hard (road grip; otherwise grass grip)
    ('open', (0.080, 0.105, 0.040), 0.0, 0.0, 0),
    ('farmland', (0.085, 0.080, 0.045), 0.0, 0.0, 0),   # soil and winter crops; the old beige read as sand
    ('meadow', (0.072, 0.115, 0.035), 0.0, 0.0, 0),
    ('forest', (0.030, 0.045, 0.020), 0.0, 0.0, 0),
    ('urban', (0.090, 0.100, 0.060), 0.0, 0.0, 1),
    ('industrial', (0.110, 0.108, 0.100), 0.1, 0.0, 1),
    ('park', (0.060, 0.120, 0.030), 0.0, 0.0, 0),
    ('paved', (0.095, 0.095, 0.092), 0.1, 0.0, 1),   # mostly parking and squares: asphalt, a touch lighter than the roads
    ('water', (0.010, 0.022, 0.025), 1.0, 0.0, 0),
    ('rail', (0.120, 0.100, 0.085), 0.0, 0.0, 0),
    ('path', (0.170, 0.140, 0.100), 0.0, 0.0, 0),
    ('footway', (0.140, 0.137, 0.130), 0.1, 0.0, 1),
    ('road', (0.065, 0.065, 0.068), 0.15, 0.06, 1),
    ('building', (0.120, 0.115, 0.110), 0.0, 0.0, 1),
    ('scrub', (0.050, 0.075, 0.030), 0.0, 0.0, 0),
    ('bare', (0.130, 0.115, 0.085), 0.0, 0.0, 0),
]
L = {name: k for k, (name, *_) in enumerate(LAND)}
LANDUSE = {'farmland': 'farmland', 'orchard': 'meadow', 'vineyard': 'meadow', 'meadow': 'meadow', 'grass': 'meadow', 'forest': 'forest',
           'residential': 'urban', 'farmyard': 'urban', 'industrial': 'industrial', 'commercial': 'industrial', 'retail': 'industrial',
           'railway': 'rail', 'construction': 'bare', 'brownfield': 'bare', 'quarry': 'bare', 'landfill': 'bare', 'allotments': 'park',
           'cemetery': 'park', 'recreation_ground': 'park', 'village_green': 'park', 'flowerbed': 'park', 'greenfield': 'meadow',
           'basin': 'water', 'reservoir': 'water', 'garages': 'paved', 'military': 'industrial', 'religious': 'urban', 'education': 'urban'}
NATURAL = {'wood': 'forest', 'scrub': 'scrub', 'heath': 'scrub', 'grassland': 'meadow', 'wetland': 'meadow', 'water': 'water', 'sand': 'bare', 'bare_rock': 'bare'}
LEISURE = {'park': 'park', 'garden': 'park', 'pitch': 'park', 'playground': 'park', 'golf_course': 'park', 'sports_centre': 'paved',
           'stadium': 'paved', 'track': 'path', 'swimming_pool': 'water', 'nature_reserve': None}
# painted in this order, so the later ones win where they overlap
ORDER = ['open', 'farmland', 'meadow', 'scrub', 'forest', 'urban', 'industrial', 'bare', 'park', 'paved', 'water', 'rail', 'path', 'footway', 'road', 'building']
WATERWAY = {'river': 12.0, 'canal': 20.0, 'stream': 3.0, 'ditch': 1.5, 'drain': 1.5}
WALL_TINTS = [(0.95, 0.88, 0.74), (0.92, 0.80, 0.62), (0.80, 0.80, 0.78), (0.93, 0.78, 0.70)]   # cream, ochre, grey, rose


# ------------------------------------------------------------------ geometry helpers (from the earlier OSM maps)

def signed_area(r):
    x, y = r[:, 0], r[:, 1]
    return 0.5 * float(np.sum(x * np.roll(y, -1) - np.roll(x, -1) * y))


def simplify(line, tol):
    """Douglas-Peucker on an open polyline (any number of columns; distances in the first two)."""
    if len(line) < 3:
        return line
    keep = np.zeros(len(line), bool)
    keep[[0, -1]] = True
    stack = [(0, len(line) - 1)]
    while stack:
        a, b = stack.pop()
        if b <= a + 1:
            continue
        d = line[b, :2] - line[a, :2]
        rel = line[a + 1:b, :2] - line[a, :2]
        length = math.hypot(*d)
        dist = np.abs(rel[:, 0] * d[1] - rel[:, 1] * d[0]) / length if length > 1e-9 else np.hypot(*rel.T)
        k = int(np.argmax(dist))
        if dist[k] > tol:
            keep[a + 1 + k] = True
            stack += [(a, a + 1 + k), (a + 1 + k, b)]
    return line[keep]


def clean_ring(r, tol=0.2, ccw=True):
    """A closed 2D outline without its repeated end, simplified, turned counter-clockwise (or clockwise); None if nothing is left."""
    if len(r) > 1 and np.allclose(r[0], r[-1]):
        r = r[:-1]
    if len(r) < 3:
        return None
    r = simplify(np.vstack([r, r[:1]]), tol)[:-1]
    if len(r) < 3 or abs(signed_area(r)) < 1.0:
        return None
    return r if (signed_area(r) > 0) == ccw else r[::-1].copy()


def triangulate(r):
    """Ear clipping of a counter-clockwise 2D outline: index triples, counter-clockwise."""
    idx = list(range(len(r)))
    tris = []
    k = 0
    stall = 0
    while len(idx) > 3 and stall < len(idx):
        n = len(idx)
        i0, i1, i2 = idx[(k - 1) % n], idx[k % n], idx[(k + 1) % n]
        a, b, c = r[i0], r[i1], r[i2]
        ear = (b[0] - a[0]) * (c[1] - a[1]) - (b[1] - a[1]) * (c[0] - a[0]) > 1e-12
        if ear:
            others = r[[j for j in idx if j not in (i0, i1, i2)]]
            if len(others):
                d1 = (b[0] - a[0]) * (others[:, 1] - a[1]) - (b[1] - a[1]) * (others[:, 0] - a[0])
                d2 = (c[0] - b[0]) * (others[:, 1] - b[1]) - (c[1] - b[1]) * (others[:, 0] - b[0])
                d3 = (a[0] - c[0]) * (others[:, 1] - c[1]) - (a[1] - c[1]) * (others[:, 0] - c[0])
                ear = not np.any((d1 >= 0) & (d2 >= 0) & (d3 >= 0))
        if ear:
            tris.append((i0, i1, i2))
            idx.pop(k % n)
            stall = 0
        else:
            k += 1
            stall += 1
    if len(idx) == 3:
        tris.append(tuple(idx))
    return tris


def chain(lines):
    """Ways joined end to end into longer roads: at each end, the unused way that carries on most nearly straight."""
    ends = {}
    for k, pts in enumerate(lines):
        for end in (0, -1):
            ends.setdefault(tuple(np.round(pts[end, :2], 2)), []).append(k)
    used, out = set(), []
    for k in range(len(lines)):
        if k in used:
            continue
        used.add(k)
        line = lines[k]
        for _ in range(2):   # grow one end, then turn the line round and grow the other
            while True:
                heading, best = line[-1, :2] - line[-2, :2], None
                for j in ends.get(tuple(np.round(line[-1, :2], 2)), []):
                    if j in used:
                        continue
                    nxt = lines[j] if np.allclose(lines[j][0, :2], line[-1, :2], atol=0.02) else lines[j][::-1]
                    step = nxt[1, :2] - nxt[0, :2]
                    cos = float(heading @ step) / (math.hypot(*heading) * math.hypot(*step) + 1e-9)
                    if cos > 0.5 and (best is None or cos > best[0]):
                        best = (cos, j, nxt)
                if best is None:
                    break
                used.add(best[1])
                line = np.vstack([line, best[2][1:]])
            line = line[::-1]
        out.append(line)
    return out


def along(pts):
    return np.r_[0, np.cumsum(np.hypot(*np.diff(pts[:, :2], axis=0).T))]


def densify(pts, spacing):
    """Points every `spacing` m or closer along a polyline, keeping its corners (any number of columns, interpolated)."""
    out = [pts[:1]]
    for a, b in zip(pts[:-1], pts[1:]):
        n = max(1, int(math.ceil(math.hypot(*(b[:2] - a[:2])) / spacing)))
        t = np.arange(1, n + 1)[:, None] / n
        out.append(a + (b - a) * t)
    return np.vstack(out)


# ------------------------------------------------------------------ the map frame and the terrain

class Frame:
    """x east, y south (so the map is not mirrored in Unreal), metres from the middle of the city; heights above the datum."""

    def __init__(self, boundary_utm):
        e, n = boundary_utm[:, 0], boundary_utm[:, 1]
        self.E0, self.N0 = round((e.min() + e.max()) / 200) * 100, round((n.min() + n.max()) / 200) * 100
        self.datum = 0.0

    def xy(self, e, n):
        return np.c_[np.asarray(e) - self.E0, self.N0 - np.asarray(n)]

    def lonlat(self, lon, lat):
        e, n = TO_UTM.transform(np.asarray(lon, 'f8'), np.asarray(lat, 'f8'))
        return self.xy(e, n)


class Terrain:
    """DGM1 averaged over 2 x 2 m: sample (j, i) is the mean height of the square around (tx0 + i STEP, ty0 + j STEP).
    Heights are kept in whole centimetres above the datum, exactly what the game reads back, so both agree to the bit."""

    def __init__(self, frame, boundary_utm):
        tiles = {tuple(int(v) for v in os.path.basename(f)[:-4].split('_')): f for f in glob.glob(os.path.join(DATA, 'dgm1', '*.tif'))}
        te = [k[0] * 1000 for k in tiles]
        tn = [k[1] * 1000 for k in tiles]
        e0 = max(min(te), math.floor((boundary_utm[:, 0].min() - MARGIN) / STEP) * STEP)
        e1 = min(max(te) + 1000, math.ceil((boundary_utm[:, 0].max() + MARGIN) / STEP) * STEP)
        n0 = max(min(tn), math.floor((boundary_utm[:, 1].min() - MARGIN) / STEP) * STEP)
        n1 = min(max(tn) + 1000, math.ceil((boundary_utm[:, 1].max() + MARGIN) / STEP) * STEP)
        self.nx, self.ny = int((e1 - e0) / STEP), int((n1 - n0) / STEP)
        h = np.full((self.ny, self.nx), np.nan, 'f4')
        k = int(STEP)
        for (te_km, tn_km), path in sorted(tiles.items()):
            a = np.array(Image.open(path), 'f4')
            assert a.shape == (1000, 1000), (path, a.shape)
            a[a < -1000] = np.nan
            a = np.nanmean(a.reshape(1000 // k, k, 1000 // k, k), axis=(1, 3))   # row 0 is the tile's north edge
            col, row = int((te_km * 1000 - e0) / STEP), int((n1 - (tn_km * 1000 + 1000)) / STEP)
            r0, c0 = max(row, 0), max(col, 0)
            r1, c1 = min(row + a.shape[0], self.ny), min(col + a.shape[1], self.nx)
            if r1 > r0 and c1 > c0:
                h[r0:r1, c0:c1] = a[r0 - row:r1 - row, c0 - col:c1 - col]
        self.valid = ~np.isnan(h)
        # beyond the survey's tiles (corners outside the city): the nearest measured height
        far = ndimage.distance_transform_edt(~self.valid, return_distances=False, return_indices=True)
        h = h[far[0], far[1]]
        frame.datum = math.floor(float(h.min())) - 1.0
        self.cm = np.round((h - frame.datum) * 100).astype(np.int32)
        assert self.cm.max() < 32000, 'heights beyond what the file can hold'
        self.z = self.cm.astype('f8') / 100
        self.tx0 = e0 + STEP / 2 - frame.E0
        self.ty0 = frame.N0 - n1 + STEP / 2
        self.frame = frame
        self.decks = None

    def at(self, x, y):
        """Bilinear, as Track::HeightAt in HoMoLapCore.cpp."""
        c = np.clip((np.asarray(x, 'f8') - self.tx0) / STEP, 0, self.nx - 1.000001)
        r = np.clip((np.asarray(y, 'f8') - self.ty0) / STEP, 0, self.ny - 1.000001)
        i, j = c.astype(int), r.astype(int)
        fc, fr = c - i, r - j
        z = self.z
        return (z[j, i] * (1 - fc) + z[j, i + 1] * fc) * (1 - fr) + (z[j + 1, i] * (1 - fc) + z[j + 1, i + 1] * fc) * fr

    def px(self, pts):
        """Map points to the land raster's pixel coordinates (PIL: x across, y down, pixel centres at + 0.5)."""
        return [((x - self.tx0) / STEP + 0.5, (y - self.ty0) / STEP + 0.5) for x, y in pts[:, :2]]

    def inside(self, pts, pad=0.0):
        x, y = pts[:, 0], pts[:, 1]
        return (x >= self.tx0 + pad) & (x <= self.tx0 + (self.nx - 1) * STEP - pad) & (y >= self.ty0 + pad) & (y <= self.ty0 + (self.ny - 1) * STEP - pad)


def encode_heights(cm):
    """Row by row, each sample as its step from the one before: one byte, or -128 and two bytes when the step is big."""
    flat = cm.ravel().astype(np.int64)
    d = np.diff(np.r_[0, flat])
    big = np.abs(d) > 127
    out = bytearray()
    starts = np.flatnonzero(big)
    prev = 0
    small = d.astype(np.int8).tobytes()
    for s in starts:
        out += small[prev:s]
        out += struct.pack('<bh', -128, int(d[s]))
        prev = s + 1
    out += small[prev:]
    return bytes(out)


def decode_heights(blob, n):
    """The game's decoder, in Python, for the self-check."""
    out = np.empty(n, np.int64)
    v, k, i = 0, 0, 0
    b = np.frombuffer(blob, np.int8)
    while i < n:
        if b[k] == -128:
            v += struct.unpack_from('<h', blob, k + 1)[0]
            k += 3
        else:
            v += int(b[k])
            k += 1
        out[i] = v
        i += 1
    return out


def encode_runs(values):
    """(value, run length) pairs, a byte and two bytes each."""
    flat = values.ravel()
    change = np.flatnonzero(np.diff(flat)) + 1
    starts = np.r_[0, change]
    lengths = np.diff(np.r_[starts, len(flat)])
    out = bytearray()
    for s, n in zip(starts, lengths):
        while n > 0:
            m = min(int(n), 65535)
            out += struct.pack('<BH', int(flat[s]), m)
            n -= m
    return bytes(out)


# ------------------------------------------------------------------ OpenStreetMap

class Osm(osmium.SimpleHandler):
    def __init__(self, box):
        super().__init__()
        self.box = box   # lon0, lat0, lon1, lat1
        self.roads, self.paths, self.rails, self.waterways, self.areas, self.trees, self.lamps, self.rows = [], [], [], [], [], [], [], []
        self.boundary = None

    def near(self, lon, lat):
        b = self.box
        return b[0] <= lon <= b[2] and b[1] <= lat <= b[3]

    def node(self, n):
        t = n.tags
        if (t.get('natural') == 'tree' or t.get('highway') == 'street_lamp') and self.near(n.location.lon, n.location.lat):
            (self.trees if t.get('natural') == 'tree' else self.lamps).append((n.location.lon, n.location.lat))

    def way(self, w):
        t = w.tags
        hw, rw, ww = t.get('highway'), t.get('railway'), t.get('waterway')
        if not (hw in WIDTH or hw in PATHS or rw in ('rail', 'tram', 'light_rail', 'narrow_gauge') or ww in WATERWAY or t.get('natural') == 'tree_row'):
            return
        try:
            ll = [(n.lon, n.lat) for n in w.nodes]
        except osmium.InvalidLocationError:
            return
        if len(ll) < 2 or not any(self.near(*p) for p in ll):
            return
        tags = dict(t)
        item = (w.id, tags, np.array(ll), [n.ref for n in w.nodes])
        if hw in WIDTH and tags.get('area') != 'yes':
            self.roads.append(item)
        elif hw in PATHS and tags.get('area') != 'yes':
            self.paths.append(item)
        elif rw:
            self.rails.append(item)
        elif ww:
            self.waterways.append(item)
        elif tags.get('natural') == 'tree_row':
            self.rows.append(item)

    def area(self, a):
        t = a.tags
        if not a.from_way() and a.orig_id() == BOUNDARY:
            self.boundary = [np.array([(n.lon, n.lat) for n in ring]) for ring in a.outer_rings()]
            return
        kind = None
        if t.get('landuse') in LANDUSE:
            kind = LANDUSE[t.get('landuse')]
        if t.get('natural') in NATURAL:
            kind = NATURAL[t.get('natural')]
        if t.get('leisure') in LEISURE and LEISURE[t.get('leisure')]:
            kind = LEISURE[t.get('leisure')]
        if t.get('amenity') == 'parking' and t.get('parking', 'surface') == 'surface':
            kind = 'paved'
        if t.get('highway') == 'pedestrian' or t.get('place') == 'square' or t.get('area:highway') in WIDTH:
            kind = 'paved' if t.get('area:highway') not in WIDTH else 'road'
        if t.get('waterway') == 'riverbank' or t.get('water'):
            kind = 'water'
        if kind is None:
            return
        try:
            rings = []
            for outer in a.outer_rings():
                ring = np.array([(n.lon, n.lat) for n in outer])
                if not np.any((ring[:, 0] >= self.box[0]) & (ring[:, 0] <= self.box[2]) & (ring[:, 1] >= self.box[1]) & (ring[:, 1] <= self.box[3])):
                    continue
                holes = [np.array([(n.lon, n.lat) for n in inner]) for inner in a.inner_rings(outer)]
                rings.append((ring, holes))
        except osmium.InvalidLocationError:
            return
        if rings:
            self.areas.append((kind, rings))


def tiles_lonlat_box():
    """The survey's terrain tiles for Erlangen as a lon/lat box: what is read from the map."""
    names = [os.path.basename(f)[:-4].split('_') for f in glob.glob(os.path.join(DATA, 'dgm1', '*.tif'))]
    assert names, 'no terrain tiles: run tools/erlangen_fetch.py first'
    e = [int(a) * 1000 for a, _ in names]
    n = [int(b) * 1000 for _, b in names]
    back = Transformer.from_crs(25832, 4326, always_xy=True)
    lon, lat = back.transform([min(e), min(e), max(e) + 1000, max(e) + 1000], [min(n), max(n) + 1000, min(n), max(n) + 1000])
    return min(lon), min(lat), max(lon), max(lat)


def number(text):
    try:
        return float(str(text).replace(',', '.').split()[0].rstrip('m'))
    except (ValueError, IndexError, TypeError):
        return None


def road_width(tags):
    width, lanes = number(tags.get('width')), number(tags.get('lanes'))
    if width and 3 <= width <= 30:
        return width
    if lanes and 1 <= lanes <= 8:
        return lanes * 3.2 + 1
    return WIDTH[tags['highway']]


def is_bridge(tags):
    return tags.get('bridge', 'no') not in ('no', 'abandoned') and (number(tags.get('layer')) or 1) >= 0


def is_tunnel(tags):
    return tags.get('tunnel', 'no') not in ('no',) or tags.get('covered') == 'yes'


# ------------------------------------------------------------------ roads and bridges

class Road:
    __slots__ = ('id', 'kind', 'half', 'xy', 'z', 'nodes', 'bridge', 'oneway', 'lanes')


def make_roads(osm, frame, ter):
    roads = []
    for wid, tags, ll, refs in osm.roads:
        if is_tunnel(tags):
            continue
        r = Road()
        r.id, r.kind, r.half, r.nodes = wid, tags['highway'], road_width(tags) / 2, refs
        r.xy = frame.lonlat(ll[:, 0], ll[:, 1])
        r.bridge = is_bridge(tags)
        r.oneway = tags.get('oneway') in ('yes', '1', '-1') or r.kind in ('motorway', 'motorway_link') or tags.get('junction') == 'roundabout'
        r.lanes = number(tags.get('lanes')) or (1 if r.oneway else 2)
        r.z = ter.at(r.xy[:, 0], r.xy[:, 1])
        roads.append(r)
    # bridges: DGM1 is the ground without them, so a bridge's deck runs straight from the ground at one end to the other
    ends = {}
    for k, r in enumerate(roads):
        if r.bridge:
            for end in (0, -1):
                ends.setdefault(r.nodes[end], []).append(k)
    seen, decks = set(), []
    for k, r in enumerate(roads):
        if not r.bridge or k in seen:
            continue
        group, todo = [], [k]   # bridge ways joined end to end: one deck
        while todo:
            q = todo.pop()
            if q in seen:
                continue
            seen.add(q)
            group.append(q)
            for end in (0, -1):
                todo += [m for m in ends.get(roads[q].nodes[end], []) if m not in seen]
        lines = chain([np.c_[roads[q].xy, np.full(len(roads[q].xy), q)] for q in group])
        for line in lines:
            s = along(line)
            if s[-1] < 1:
                continue
            z0, z1 = ter.at(*line[0, :2]), ter.at(*line[-1, :2])
            zl = z0 + (z1 - z0) * s / s[-1]
            tree = cKDTree(line[:, :2])
            for q in group:   # each way's points take the deck's height where they lie on this line
                d, i = tree.query(roads[q].xy)
                ok = d < 0.05
                roads[q].z = np.where(ok, zl[i], roads[q].z)
            half = max(roads[int(q)].half for q in set(line[:, 2].astype(int)))
            decks.append((line[:, :2], zl, half))
    return roads, decks


# ------------------------------------------------------------------ the land raster

def paint_land(ter, frame, osm, roads, footprints, boundary):
    land = np.full((ter.ny, ter.nx), L['open'], np.uint8)
    by_kind = {}
    for kind, rings in osm.areas:
        by_kind.setdefault(kind, []).append(rings)

    def polygon(kind, outer, holes):
        pts = ter.px(outer)
        xs, ys = [p[0] for p in pts], [p[1] for p in pts]
        x0, y0 = max(int(min(xs)) - 1, 0), max(int(min(ys)) - 1, 0)
        x1, y1 = min(int(max(xs)) + 2, ter.nx), min(int(max(ys)) + 2, ter.ny)
        if x1 <= x0 or y1 <= y0:
            return
        img = Image.new('1', (x1 - x0, y1 - y0), 0)
        draw = ImageDraw.Draw(img)
        draw.polygon([(x - x0, y - y0) for x, y in pts], fill=1)
        for hole in holes:
            draw.polygon([(x - x0, y - y0) for x, y in ter.px(hole)], fill=0)
        m = np.array(img, bool)
        land[y0:y1, x0:x1][m] = L[kind]

    lines = Image.new('L', (ter.nx, ter.ny), 255)   # 255: nothing; the lines are drawn on their own and merged in order
    for kind in ORDER:
        for rings in by_kind.get(kind, []):
            for outer, holes in rings:
                polygon(kind, frame.lonlat(outer[:, 0], outer[:, 1]), [frame.lonlat(h[:, 0], h[:, 1]) for h in holes])
        if kind in ('water', 'rail', 'path', 'footway', 'road', 'building'):
            draw = ImageDraw.Draw(lines)
            lines.paste(255, (0, 0, ter.nx, ter.ny))
            if kind == 'water':
                for _, tags, ll, _ in osm.waterways:
                    if not is_tunnel(tags) and tags.get('waterway') in WATERWAY:
                        w = number(tags.get('width')) or WATERWAY[tags['waterway']]
                        draw.line(ter.px(frame.lonlat(ll[:, 0], ll[:, 1])), fill=0, width=max(1, int(round(w / STEP))), joint='curve')
            elif kind == 'rail':
                for _, tags, ll, _ in osm.rails:
                    if not is_tunnel(tags) and not is_bridge(tags):
                        draw.line(ter.px(frame.lonlat(ll[:, 0], ll[:, 1])), fill=0, width=max(1, int(round(4.0 / STEP))), joint='curve')
            elif kind in ('path', 'footway'):
                for _, tags, ll, _ in osm.paths:
                    hw = tags['highway']
                    paved = tags.get('surface', 'paved' if hw in ('footway', 'cycleway', 'pedestrian', 'steps') else 'unpaved')
                    is_paved = paved in ('paved', 'asphalt', 'concrete', 'paving_stones', 'sett', 'concrete:plates', 'cobblestone')
                    if (kind == 'footway') == is_paved and not is_tunnel(tags) and not is_bridge(tags):
                        draw.line(ter.px(frame.lonlat(ll[:, 0], ll[:, 1])), fill=0, width=max(1, int(round(PATHS[hw] / STEP))), joint='curve')
            elif kind == 'road':
                for r in roads:
                    if not r.bridge:
                        draw.line(ter.px(r.xy), fill=0, width=max(1, int(round(2 * r.half / STEP))), joint='curve')
            elif kind == 'building':
                for ring in footprints:
                    draw.polygon(ter.px(ring), fill=0)
            land[np.array(lines) == 0] = L[kind]
    # where the game sinks its ground under the road meshes: every ground triangle a road lies on (up to a grid diagonal
    # beyond the road's edge), so no ground pokes through the drawn road
    sink = Image.new('1', (ter.nx, ter.ny), 0)
    draw = ImageDraw.Draw(sink)
    for r in roads:
        if not r.bridge:
            draw.line(ter.px(r.xy), fill=1, width=int(math.ceil((2 * r.half + 6.0) / STEP)), joint='curve')
            for end in (r.xy[0], r.xy[-1]):   # and round the ends, as the meshes are
                (cx, cy), = ter.px(end[None])
                rad = (r.half + 3.0) / STEP
                draw.ellipse([cx - rad, cy - rad, cx + rad, cy + rad], fill=1)
    return land, np.array(sink, bool)


# ------------------------------------------------------------------ LoD2 buildings

SURFACE = re.compile(r'<bldg:(GroundSurface|WallSurface|RoofSurface)\b.*?</bldg:\1>', re.S)
POSLIST = re.compile(r'<gml:(exterior|interior)>\s*<gml:LinearRing>\s*<gml:posList[^>]*>([^<]+)</gml:posList>', re.S)
BUILDING = re.compile(r'<bldg:Building gml:id="([^"]+)">(.*?)</bldg:Building>', re.S)


def read_lod2(frame, ter):
    """Every building in the LoD2 tiles over the terrain: [(id, [(kind, exterior xyz, [interior xyz])])], map frame."""
    seen, out = set(), []
    for path in sorted(glob.glob(os.path.join(DATA, 'lod2', '*.gml'))):
        text = open(path, encoding='utf-8').read()
        for gid, body in BUILDING.findall(text):
            if gid in seen:
                continue
            seen.add(gid)
            if '<bldg:function>53001_' in body:   # ALKIS traffic structures (bridges): solid blocks here; OSM's decks draw them
                continue
            surfaces = []
            for m in SURFACE.finditer(body):
                rings = {'exterior': None, 'interior': []}
                for which, pos in POSLIST.findall(m.group(0)):
                    v = np.array(pos.split(), 'f8').reshape(-1, 3)
                    v = np.c_[v[:, 0] - frame.E0, frame.N0 - v[:, 1], v[:, 2] - frame.datum]
                    if which == 'exterior':
                        if rings['exterior'] is not None:   # a multi-surface: each polygon on its own
                            surfaces.append((m.group(1), rings['exterior'], rings['interior']))
                            rings = {'exterior': None, 'interior': []}
                        rings['exterior'] = v
                    else:
                        rings['interior'].append(v)
                if rings['exterior'] is not None:
                    surfaces.append((m.group(1), rings['exterior'], rings['interior']))
            if '<bldg:function>51009_1610' in body:   # ALKIS 'Ueberdachung', a roof without walls (carports, petrol stations): LoD2 closes it as a box
                surfaces = [('CanopyGround' if k == 'GroundSurface' else k, e, h) for k, e, h in surfaces if k != 'WallSurface']
            elif re.search(r'<bldg:function>5[12]', body):   # other structures (walls, a lock, a monument): concrete, no windows
                surfaces = [('StructureWall' if k == 'WallSurface' else k, e, h) for k, e, h in surfaces]
            if not surfaces:
                continue
            middle = np.vstack([s[1] for s in surfaces]).mean(0)
            if not ter.inside(middle[None, :2], 4.0)[0] or not ter.valid[int((middle[1] - ter.ty0) / STEP), int((middle[0] - ter.tx0) / STEP)]:
                continue
            out.append((gid, surfaces))
    return out


def newell(v):
    """The plane normal of a polygon (its vertices in order), by Newell's method."""
    a, b = v, np.roll(v, -1, axis=0)
    n = np.array([np.sum((a[:, 1] - b[:, 1]) * (a[:, 2] + b[:, 2])), np.sum((a[:, 2] - b[:, 2]) * (a[:, 0] + b[:, 0])), np.sum((a[:, 0] - b[:, 0]) * (a[:, 1] + b[:, 1]))])
    length = np.linalg.norm(n)
    return n / length if length > 1e-12 else None


def polygon_triangles(v, normal):
    """Triangles of a planar 3D polygon (no repeated end), facing `normal`: index triples."""
    axis = int(np.argmax(np.abs(normal)))
    keep = [k for k in range(3) if k != axis]
    p = v[:, keep]
    # dropping x or z keeps a polygon counter-clockwise about +x or +z; dropping y turns it round (x cross z = -y)
    if (normal[axis] > 0) == (axis == 1):
        p = p[:, ::-1]
    flip = signed_area(p) < 0
    if flip:
        p = p[::-1]
    tris = triangulate(p)
    n = len(v)
    return [((n - 1 - a, n - 1 - b, n - 1 - c) if flip else (a, b, c)) for a, b, c in tris]


def building_prims(buildings, ground_drop=2.0):
    """Per material: walls with the windows texture (four tints, by building), pitched roofs, flat roofs. glTF axes (x, up, y)."""
    prims = {}

    def add(material, v, n, uv, tris):
        p, nn, u, idx = prims.setdefault(material, ([], [], [], []))
        base = len(p)
        p += [(x, z, y) for x, y, z in v]
        nn += [(n[0], n[2], n[1])] * len(v)
        u += list(map(tuple, uv))
        for a, b, c in tris:   # glTF: counter-clockwise seen from the front, i.e. (b - a) x (c - a) along the normal in (x, up, y)
            pa, pb, pc = (np.array(p[base + q]) for q in (a, b, c))
            if np.dot(np.cross(pb - pa, pc - pa), (n[0], n[2], n[1])) < 0:
                b, c = c, b
            idx += [base + a, base + b, base + c]

    def post(x, y, z0, z1, r=0.08):
        """A canopy's post: four faces of a thin square column."""
        for nx, ny in ((1, 0), (0, 1), (-1, 0), (0, -1)):
            cx, cy, tx, ty = x + nx * r, y + ny * r, -ny * r, nx * r
            v = np.array([(cx - tx, cy - ty, z0), (cx + tx, cy + ty, z0), (cx + tx, cy + ty, z1), (cx - tx, cy - ty, z1)])
            add('ErlRoofFlat', v, np.array([nx, ny, 0.0]), np.c_[[0, 0.16, 0.16, 0], v[:, 2] / 3.0], [(0, 1, 2), (0, 2, 3)])
    for gid, surfaces in buildings:
        rng = random.Random(gid)
        wall = f'ErlWall{rng.randrange(len(WALL_TINTS))}'
        ground = min(float(s[1][:, 2].min()) for s in surfaces)
        canopy = any(s[0] == 'CanopyGround' for s in surfaces)
        for kind, ext, holes in surfaces:
            if kind == 'GroundSurface':
                continue
            v = ext[:-1] if np.allclose(ext[0], ext[-1]) else ext
            if len(v) < 3:
                continue
            if kind == 'CanopyGround':   # posts at the outline's corners, up to the roof's lowest edge
                roofs = [float(r[1][:, 2].min()) for r in surfaces if r[0] == 'RoofSurface']
                top = min(roofs) if roofs else ground + 2.5
                d0, d1 = v[:, :2] - np.roll(v[:, :2], 1, axis=0), np.roll(v[:, :2], -1, axis=0) - v[:, :2]
                turn = np.abs(np.arctan2(d0[:, 0] * d1[:, 1] - d0[:, 1] * d1[:, 0], np.einsum('ij,ij->i', d0, d1)))
                for (x, y, _), t in zip(v, turn):
                    if t > math.radians(30):
                        post(x, y, ground - ground_drop, top)
                continue
            n = newell(v)
            if n is None:
                continue
            if kind == 'RoofSurface':
                if n[2] < 0:
                    n = -n
                flat = n[2] > 0.97
                if flat:
                    uv = v[:, :2] / 4.0
                else:   # the tiles run along the eaves and down the slope
                    eaves = np.array([-n[1], n[0], 0.0]) / max(math.hypot(n[0], n[1]), 1e-9)
                    down = np.cross(n, eaves)
                    uv = np.c_[v @ eaves, v @ down] / 2.5
                add('ErlRoofFlat' if flat else 'ErlRoof', v, n, uv, polygon_triangles(v, n))
                if canopy:   # open beneath: seen from below too
                    add('ErlRoofFlat', v, -n, uv, polygon_triangles(v, -n))
            else:
                n = -n   # the map frame mirrors north to south, so LoD2's outward rings come out facing into the building
                v = v.copy()
                v[np.abs(v[:, 2] - ground) < 0.05, 2] -= ground_drop   # the wall's foot into the ground: no gaps on a slope
                horizontal = np.array([-n[1], n[0], 0.0]) / max(math.hypot(n[0], n[1]), 1e-9)
                uv = np.c_[v @ horizontal, -v[:, 2]] / 3.0
                add('CitySidewalk' if kind == 'StructureWall' else wall, v, n, uv, polygon_triangles(v, n))
    return [(m, np.array(p, 'f8'), np.array(n, 'f8'), np.array(u, 'f8'), idx) for m, (p, n, u, idx) in prims.items() if idx]


def footprints(buildings, kinds=('GroundSurface',)):
    """The ground outlines in 2D, counter-clockwise (solid inside), and the holes in them clockwise. Canopies ('CanopyGround')
    only when asked: they are no walls."""
    rings = []
    for _, surfaces in buildings:
        for kind, ext, holes in surfaces:
            if kind not in kinds:
                continue
            r = clean_ring(ext[:, :2], 0.2, True)
            if r is not None:
                rings.append(r)
            for h in holes:
                r = clean_ring(h[:, :2], 0.2, False)
                if r is not None:
                    rings.append(r)
    return rings


# ------------------------------------------------------------------ road meshes

def road_mesh(r, ter, z_lift=0.012):
    """A road as a ribbon on the ground (each edge at the ground's height there) or, on a bridge, on its deck."""
    pts = densify(np.c_[r.xy, r.z], STEP)
    d = np.gradient(pts[:, :2], axis=0)
    length = np.maximum(np.hypot(*d.T), 1e-9)
    nx, ny = -d[:, 1] / length, d[:, 0] / length
    left = pts[:, :2] - np.c_[nx, ny] * r.half
    right = pts[:, :2] + np.c_[nx, ny] * r.half
    if r.bridge:
        zl = zr = pts[:, 2]
    else:
        zl, zr = ter.at(left[:, 0], left[:, 1]), ter.at(right[:, 0], right[:, 1])
    n = len(pts)
    p = np.empty((2 * n, 3))
    p[0::2] = np.c_[left[:, 0], zl + z_lift, left[:, 1]]
    p[1::2] = np.c_[right[:, 0], zr + z_lift, right[:, 1]]
    idx = []
    for i in range(n - 1):
        idx += [2 * i, 2 * i + 2, 2 * i + 1, 2 * i + 1, 2 * i + 2, 2 * i + 3]
    # round ends, so junctions have no gaps
    for c, zc in ((pts[0], pts[0, 2] if r.bridge else ter.at(*pts[0, :2])), (pts[-1], pts[-1, 2] if r.bridge else ter.at(*pts[-1, :2]))):
        base = len(p)
        ring = [(c[0], zc + z_lift, c[1])]
        for a in np.linspace(0, 2 * np.pi, 13)[:-1]:
            x, y = c[0] + r.half * math.cos(a), c[1] + r.half * math.sin(a)
            ring.append((x, (zc if r.bridge else ter.at(x, y)) + z_lift, y))
        p = np.vstack([p, ring])
        for k in range(12):
            idx += [base, base + 1 + k, base + 1 + (k + 1) % 12]
    return p, idx


def dashes(r, ter, every=9.0, dash=3.0, width=0.12):
    """The broken centre line of a two-way road with two or more lanes."""
    pts = densify(np.c_[r.xy, r.z], 1.0)
    s = along(pts)
    quads = []
    for s0 in np.arange(every / 2, s[-1] - dash, every):
        a = np.array([np.interp(s0, s, pts[:, k]) for k in range(3)])
        b = np.array([np.interp(s0 + dash, s, pts[:, k]) for k in range(3)])
        t = (b[:2] - a[:2]) / max(np.hypot(*(b[:2] - a[:2])), 1e-9)
        nrm = np.array([-t[1], t[0]]) * width / 2
        za = a[2] if r.bridge else ter.at(*a[:2])
        zb = b[2] if r.bridge else ter.at(*b[:2])
        quads.append([(a[0] - nrm[0], za + 0.022, a[1] - nrm[1]), (a[0] + nrm[0], za + 0.022, a[1] + nrm[1]),
                      (b[0] - nrm[0], zb + 0.022, b[1] - nrm[1]), (b[0] + nrm[0], zb + 0.022, b[1] + nrm[1])])
    return quads


def deck_mesh(line, zl, half):
    """A bridge's slab under its road: its outer faces from the underside up to a 0.9 m parapet, the parapets' inner faces
    and tops, the underside and the top. Normals as glTF (x, up, y)."""
    pts = densify(np.c_[line, zl], STEP)
    d = np.gradient(pts[:, :2], axis=0)
    length = np.maximum(np.hypot(*d.T), 1e-9)
    nx, ny = -d[:, 1] / length, d[:, 0] / length
    p, nn, idx = [], [], []
    w = half + 0.3

    def strip(off0, z0, off1, z1, normal):
        """A band along the bridge between two lines across it (offset from the middle, height above the deck)."""
        base = len(p)
        for k in range(len(pts)):
            for off, z in ((off0, z0), (off1, z1)):
                p.append((pts[k, 0] + nx[k] * off, pts[k, 2] + z, pts[k, 1] + ny[k] * off))
                nn.append(normal(k))
        for k in range(len(pts) - 1):
            a, b = base + 2 * k, base + 2 * k + 2
            idx.extend([a, b, a + 1, a + 1, b, b + 1])
    for side in (-1, 1):
        strip(side * w, -1.2, side * w, 0.9, lambda k, s=side: (nx[k] * s, 0.0, ny[k] * s))
        strip(side * (w - 0.25), 0.0, side * (w - 0.25), 0.9, lambda k, s=side: (-nx[k] * s, 0.0, -ny[k] * s))
        strip(side * (w - 0.25), 0.9, side * w, 0.9, lambda k: (0.0, 1.0, 0.0))
    strip(-w, -1.2, w, -1.2, lambda k: (0.0, -1.0, 0.0))
    strip(-w, -0.03, w, -0.03, lambda k: (0.0, 1.0, 0.0))   # the top, under the road: the road tiles cast no shadow
    return np.array(p), np.array(nn, 'f8'), idx


def fix_winding(p, n, idx):
    """Flip any triangle whose front (glTF: counter-clockwise) does not face its vertex normal."""
    idx = np.array(idx).reshape(-1, 3)
    a, b, c = p[idx[:, 0]], p[idx[:, 1]], p[idx[:, 2]]
    wrong = np.einsum('ij,ij->i', np.cross(b - a, c - a), n[idx[:, 0]]) < 0
    idx[wrong] = idx[wrong][:, [0, 2, 1]]
    return idx.ravel().tolist()


def barrier_mesh(ring, ter, height=1.1):
    pts = densify(np.c_[ring, np.zeros(len(ring))], 4.0)
    z = ter.at(pts[:, 0], pts[:, 1])
    p, nn, idx = [], [], []
    for k in range(len(pts) - 1):
        a, b = pts[k], pts[k + 1]
        t = (b[:2] - a[:2]) / max(np.hypot(*(b[:2] - a[:2])), 1e-9)
        out = (-t[1], t[0])   # facing into the city; its material shows both sides
        base = len(p)
        p += [(a[0], z[k] - 0.3, a[1]), (b[0], z[k + 1] - 0.3, b[1]), (a[0], z[k] + height, a[1]), (b[0], z[k + 1] + height, b[1])]
        nn += [(out[0], 0, out[1])] * 4
        idx += [base, base + 2, base + 1, base + 1, base + 2, base + 3]
    p, nn = np.array(p), np.array(nn, 'f8')
    return p, nn, fix_winding(p, nn, idx)


# ------------------------------------------------------------------ the timed circuit

def radius_min(line):
    """The tightest radius along a closed 1 m line, measured as the game measures its curvature (Track::K, 3 m each way)."""
    a = np.roll(line[:, :2], 3, axis=0)
    b = np.roll(line[:, :2], -3, axis=0)
    h1 = np.arctan2(line[:, 1] - a[:, 1], line[:, 0] - a[:, 0])
    h2 = np.arctan2(b[:, 1] - line[:, 1], b[:, 0] - line[:, 0])
    turn = np.abs((h2 - h1 + np.pi) % (2 * np.pi) - np.pi)
    return float(3.0 / max(turn.max(), 1e-9))   # the turn over 3 m


def circuit(frame, roads, land, ter):
    nodes, graph = {}, {}
    for r in roads:
        if r.kind not in CIRCUIT_COST or len(r.nodes) != len(r.xy):
            continue
        for a, b, pa, pb, za, zb in zip(r.nodes, r.nodes[1:], r.xy, r.xy[1:], r.z, r.z[1:]):
            nodes[a], nodes[b] = (*pa, za), (*pb, zb)
            length = math.hypot(*(pb - pa))
            graph.setdefault(a, []).append((b, length * CIRCUIT_COST[r.kind]))
            graph.setdefault(b, []).append((a, length * CIRCUIT_COST[r.kind]))
    solid = land == L['building']
    clearance = ndimage.distance_transform_edt(~solid) * STEP
    focus = frame.lonlat([FOCUS[1]], [FOCUS[0]])[0]
    xy = {k: np.array(v[:2]) for k, v in nodes.items()}

    def dijkstra(src, banned=()):
        dist, prev, heap = {src: 0.0}, {}, [(0.0, src)]
        while heap:
            d, u = heapq.heappop(heap)
            if d > dist[u]:
                continue
            for v, cost in graph[u]:
                if v in banned or d + cost >= dist.get(v, 1e18):
                    continue
                dist[v], prev[v] = d + cost, u
                heapq.heappush(heap, (d + cost, v))
        return dist, prev

    def path(prev, src, dst):
        out = [dst]
        while out[-1] != src:
            out.append(prev[out[-1]])
        return out[::-1]

    def resample(loop3, spacing=1.0):
        loop = np.vstack([loop3, loop3[:1]])
        s = along(loop)
        keep = np.r_[True, np.diff(s) > 1e-6]
        loop, s = loop[keep], s[keep]
        t = np.linspace(0, s[-1], int(round(s[-1] / spacing)), endpoint=False)
        return np.column_stack([np.interp(t, s, loop[:, k]) for k in range(3)])
    junctions = [n for n, links in graph.items() if len(links) >= 3 and np.hypot(*(xy[n] - focus)) < REACH]
    rng = random.Random(3)
    best, best_score = None, -1e9
    for a in rng.sample(junctions, min(24, len(junctions))):
        dist, prev = dijkstra(a)
        far = [b for b in junctions if b in dist and 700 < np.hypot(*(xy[b] - xy[a])) < LAP / 2]
        for b in rng.sample(far, min(8, len(far))):
            out = path(prev, a, b)
            dist2, prev2 = dijkstra(b, banned=set(out[1:-1]))
            if a not in dist2:
                continue
            loop = out + path(prev2, b, a)[1:-1]
            line = resample(np.array([nodes[n] for n in loop]))
            if not 2200 < len(line) < LAP * 1.3:
                continue
            k = np.exp(-0.5 * (np.arange(-30, 31) / 10.0) ** 2)   # smooth the corners: a 10 m wide Gaussian along the lap
            k /= k.sum()
            line = np.column_stack([np.convolve(np.r_[c[-30:], c, c[:30]], k, 'valid') for c in line.T])
            if radius_min(line) < MIN_RADIUS:   # a junction's square corner the AI can't take at speed
                continue
            coarse = line[::2, :2]   # no part of the lap may run close beside another part of it
            gap = [abs(i - j) for i, j in cKDTree(coarse).query_pairs(25.0)]
            if any(min(g, len(coarse) - g) * 2 > 80 for g in gap):
                continue
            ix = np.clip(((line[:, 0] - ter.tx0) / STEP + 0.5).astype(int), 0, ter.nx - 1)
            iy = np.clip(((line[:, 1] - ter.ty0) / STEP + 0.5).astype(int), 0, ter.ny - 1)
            room = clearance[iy, ix]
            if room.min() < 3.5:
                continue
            score = min(np.percentile(room, 2), 9) * 2 - abs(len(line) - LAP) / 250
            if score > best_score:
                best, best_score = (line, float(room.min())), score
    if best is None:
        raise SystemExit('no circuit found')
    line, room = best
    d = np.roll(line[:, :2], -3, axis=0) - np.roll(line[:, :2], 3, axis=0)   # the lap starts on the straightest stretch
    heading = np.unwrap(np.arctan2(d[:, 1], d[:, 0]))
    bend = np.abs(np.roll(heading, -30) - np.roll(heading, 30))
    start = int(np.argmin(bend + np.roll(bend, 30)))
    return np.roll(line, -start, axis=0), room


# ------------------------------------------------------------------ trees and lamps

def scatter(frame, ter, land, osm, roads, boundary):
    rng = random.Random(11)
    # trees only in the city and 300 m round it: beyond that the car can't go, and the game hides trees 800 m away
    city = Image.new('1', (ter.nx, ter.ny), 0)
    ImageDraw.Draw(city).polygon(ter.px(boundary), fill=1)
    near_city = ndimage.distance_transform_edt(~np.array(city, bool)) * STEP <= TREE_MARGIN
    free_land = np.isin(land, [L[k] for k in ('open', 'farmland', 'meadow', 'forest', 'urban', 'park', 'scrub', 'bare', 'industrial')]) & near_city

    def free(x, y):
        i, j = int((x - ter.tx0) / STEP + 0.5), int((y - ter.ty0) / STEP + 0.5)
        return 0 <= j < ter.ny and 0 <= i < ter.nx and free_land[j, i] and ter.valid[j, i]
    spots = [p for p in frame.lonlat(*np.array(osm.trees).T) if free(*p)] if osm.trees else []
    mapped = len(spots)
    for _, _, ll, _ in osm.rows:
        line = densify(frame.lonlat(ll[:, 0], ll[:, 1]), 8.0)
        spots += [p for p in line if free(*p)]
    # woods and parks: one tree every 12 m (woods) or 18 m (parks), jittered
    for cls, step in ((L['forest'], 12.0), (L['park'], 18.0)):
        k = int(step / STEP)
        jj, ii = np.nonzero(land[k // 2::k, k // 2::k] == cls)
        for j, i in zip(jj, ii):
            x = ter.tx0 + (i * k + k // 2) * STEP + rng.uniform(-3.5, 3.5)
            y = ter.ty0 + (j * k + k // 2) * STEP + rng.uniform(-3.5, 3.5)
            if free(x, y):
                spots.append((x, y))
    poles, tops = [], []
    for fx, fy in spots:
        r = rng.uniform(2.2, 3.8)
        poles.append((fx, fy, 0, 0.4, 0.4, r + 1.6))
        tops.append((fx, fy, r + 3.2, r))
    # lamps: the mapped ones, and along the town's streets every 32 m where none is mapped near
    lamp_at = [p for p in frame.lonlat(*np.array(osm.lamps).T)] if osm.lamps else []
    near = cKDTree(lamp_at) if lamp_at else None
    urban = np.isin(land, [L['urban'], L['industrial'], L['paved']])
    for r in roads:
        if r.kind in ('service', 'motorway', 'motorway_link', 'trunk') or r.bridge:
            continue
        pts = densify(np.c_[r.xy, r.z], 4.0)
        s = along(pts)
        for k, at in enumerate(np.arange(16, s[-1] - 10, 32)):
            i = min(int(np.searchsorted(s, at)), len(pts) - 1)
            d = pts[min(i + 1, len(pts) - 1), :2] - pts[max(i - 1, 0), :2]
            d /= max(np.hypot(*d), 1e-9)
            side = 1 if k % 2 else -1
            fx, fy = pts[i, :2] + np.array([-d[1], d[0]]) * side * (r.half + 1.2)
            gi, gj = int((fx - ter.tx0) / STEP + 0.5), int((fy - ter.ty0) / STEP + 0.5)
            if not (0 <= gj < ter.ny and 0 <= gi < ter.nx and urban[gj, gi]):
                continue
            if near is not None and near.query((fx, fy))[0] < 20:
                continue
            lamp_at.append((fx, fy))
    blocked = np.isin(land, [L['building'], L['road'], L['water']])
    for fx, fy in lamp_at:
        gi, gj = int((fx - ter.tx0) / STEP + 0.5), int((fy - ter.ty0) / STEP + 0.5)
        if 0 <= gj < ter.ny and 0 <= gi < ter.nx and not blocked[gj, gi]:
            poles.append((fx, fy, 0, 0.2, 0.2, 8.0))
            poles.append((fx, fy, 8.0, 0.7, 0.7, 0.18))   # the lamp head, overhead
    return poles, tops, mapped


# ------------------------------------------------------------------ output

def write_bin(path, arrays):
    """Named arrays: 'FSLB', version, count, then per array a 16-byte name, a type letter, 3 pad bytes, a 64-bit count and the data."""
    os.makedirs(os.path.dirname(path), exist_ok=True)
    kinds = {'d': '<f8', 'f': '<f4', 'i': '<i4', 'b': 'u1'}
    with open(path, 'wb') as out:
        out.write(b'FSLB' + struct.pack('<II', 1, len(arrays)))
        for name, (kind, values) in arrays.items():
            raw = values if isinstance(values, bytes) else np.ascontiguousarray(np.asarray(values, kinds[kind]).ravel()).tobytes()
            count = len(raw) // np.dtype(kinds[kind]).itemsize
            out.write(name.encode().ljust(16, b'\0') + kind.encode() + b'\0\0\0' + struct.pack('<Q', count) + raw)


def lines_with_breaks(lines, cols):
    out = []
    for line in lines:
        out += [tuple(p[:cols]) for p in line] + [(1e9,) * cols]
    return np.array(out[:-1] if out else [(0.0,) * cols])


def plan(path, ter, land, line, boundary):
    colours = np.array([(np.array(c) ** (1 / 2.2) * 255) for _, c, *_ in LAND], 'u1')
    img = Image.fromarray(colours[land[::4, ::4]])
    shade = ter.z[::4, ::4]
    gy, gx = np.gradient(shade)
    light = np.clip(1 - 0.6 * (gx - gy), 0.5, 1.5)
    img = Image.fromarray(np.clip(np.array(img, 'f4') * light[..., None], 0, 255).astype('u1'))
    draw = ImageDraw.Draw(img)
    f = lambda pts: [((x - ter.tx0) / STEP / 4, (y - ter.ty0) / STEP / 4) for x, y in pts[:, :2]]
    draw.line(f(np.vstack([boundary, boundary[:1]])), fill=(230, 60, 50), width=3)
    draw.line(f(np.vstack([line, line[:1]])), fill=(255, 220, 60), width=4)
    img.save(path)


def noise_texture(path, size=512, seed=5):
    """Noise that tiles, for the ground's material, mean 0.5: red only fine grain (sampled every 4 m: anything coarser
    showed as a 4 m parquet), green soft blotches (sampled every 36 m)."""
    rng = np.random.default_rng(seed)

    def layers(*pairs):
        out = np.zeros((size, size))
        for sigma, weight in pairs:
            layer = ndimage.gaussian_filter(rng.standard_normal((size, size)), sigma, mode='wrap')
            out += weight * layer / layer.std()
        return np.clip(0.5 + 0.18 * out, 0, 1)
    rgb = np.dstack([layers((1.0, 0.6), (3.0, 0.4)), layers((8.0, 0.4), (32.0, 0.6)), np.full((size, size), 0.5)])
    Image.fromarray((rgb * 255).astype('u1')).save(path)


def self_check():
    square = np.array([(0, 0), (4, 0), (4, 4), (2, 1), (0, 4)], float)   # a notched outline
    area = sum(abs(signed_area(square[list(t)])) for t in triangulate(square))
    assert abs(area - signed_area(square)) < 1e-9, area
    a, b, side = np.array([(0, 0), (10, 0.0)]), np.array([(20, 0), (10, 0.0)]), np.array([(10, 0), (10, 9.0)])
    assert sorted(len(line) for line in chain([a, b, side])) == [2, 3]
    t = np.linspace(0, 2 * np.pi, 629, endpoint=False)   # a circle of radius 100 m, 1 m apart
    assert abs(radius_min(np.c_[100 * np.cos(t), 100 * np.sin(t), 0 * t]) - 100) < 1
    cm = np.array([[0, 5, 300, 299], [31000, 30990, 0, 2]])
    assert np.array_equal(decode_heights(encode_heights(cm), cm.size), cm.ravel())
    # a wall standing on the x axis, facing -y (map south... north): its triangles face its normal
    wall = np.array([(0, 0, 0), (4, 0, 0), (4, 0, 3), (0, 0, 3.0)])
    n = newell(wall)
    for t in polygon_triangles(wall, n):
        pa, pb, pc = wall[list(t)]
        assert np.dot(np.cross(pb - pa, pc - pa), n) > 0
    # LoD2's south wall of a box north of y = 0, its ring counter-clockwise from outside, mirrored as Frame mirrors it:
    # in the map the box lies at y < 0, so the wall must face +y (glTF's third axis)
    (_, p, _, _, idx), = building_prims([('box', [('WallSurface', np.array([(0, 0, 0), (4, 0, 0), (4, 0, 3), (0, 0, 3.0)]) * [1, -1, 1], [])])])
    assert all(np.cross(p[b] - p[a], p[c] - p[a])[2] > 0 for a, b, c in np.reshape(idx, (-1, 3)))
    # a canopy: no walls to hit, four posts, its roof seen from above and below
    square = np.array([(0, 0, 0), (4, 0, 0), (4, 4, 0), (0, 4, 0.0)])
    shed = [('box', [('CanopyGround', square, []), ('RoofSurface', square + [0, 0, 3], [])])]
    assert footprints(shed) == [] and len(footprints(shed, ('GroundSurface', 'CanopyGround'))) == 1
    prims = {m: (p, n) for m, p, n, _, _ in building_prims(shed)}
    assert set(prims) == {'ErlRoofFlat'} and np.sum(prims['ErlRoofFlat'][1][:, 1] > 0.9) == 4 and np.sum(prims['ErlRoofFlat'][1][:, 1] < -0.9) == 4
    assert len(prims['ErlRoofFlat'][0]) == 4 + 4 + 4 * 4 * 4   # two roofs, four posts of four faces
    assert [m for m, *_ in building_prims([('wall', [('StructureWall', np.array([(0, 0, 0), (4, 0, 0), (4, 0, 2), (0, 0, 2.0)]), [])])])] == ['CitySidewalk']
    roof = np.array([(0, 0, 5), (0, 4, 5), (4, 4, 5), (4, 0, 5.0)])
    n = newell(roof)
    assert n[2] < 0 and all(np.dot(np.cross(roof[t[1]] - roof[t[0]], roof[t[2]] - roof[t[0]]), n) > 0 for t in polygon_triangles(roof, n))


def main():
    self_check()
    t0 = time.time()
    log = lambda *a: print(f'[{time.time() - t0:6.0f} s]', *a, flush=True)
    os.makedirs(OUT, exist_ok=True)
    osm = Osm(tiles_lonlat_box())
    osm.apply_file(os.path.join(DATA, 'osm', 'mittelfranken.osm.pbf'), locations=True)
    assert osm.boundary and len(osm.boundary) == 1, 'the city boundary should be one ring'
    log(f'osm: {len(osm.roads)} roads, {len(osm.paths)} paths, {len(osm.areas)} areas, {len(osm.trees)} trees, {len(osm.lamps)} lamps')
    bound_ll = osm.boundary[0]
    bound_utm = np.c_[TO_UTM.transform(bound_ll[:, 0], bound_ll[:, 1])]
    frame = Frame(bound_utm)
    ter = Terrain(frame, bound_utm)
    log(f'terrain {ter.nx} x {ter.ny} at {STEP} m, datum {frame.datum} m, {ter.valid.mean() * 100:.0f} % measured, heights {ter.z.min():.2f}-{ter.z.max():.2f} m above it')
    boundary = clean_ring(frame.xy(*bound_utm.T), 0.5, False)   # clockwise: the edges are solid outside
    roads, decks = make_roads(osm, frame, ter)
    log(f'{len(roads)} roads, {sum(r.bridge for r in roads)} on {len(decks)} bridges')
    buildings = read_lod2(frame, ter)
    rings = footprints(buildings)
    log(f'LoD2: {len(buildings)} buildings, {len(rings)} ground outlines')
    # the ground under a canopy stays painted as a building's: the colour, and the circuit's distance from buildings, as before
    land, sink = paint_land(ter, frame, osm, roads, footprints(buildings, ('GroundSurface', 'CanopyGround')), boundary)
    log('land: ' + ', '.join(f'{LAND[k][0]} {np.mean(land == k) * 100:.1f}%' for k in range(len(LAND)) if np.any(land == k)))
    line, room = circuit(frame, roads, land, ter)
    half = float(np.clip(room - 1.5, 3.0, 4.5))
    log(f'circuit {len(line)} m, lane half {half:.1f} m, closest building {room:.1f} m, tightest corner {radius_min(line):.1f} m, climb {line[:, 2].max() - line[:, 2].min():.1f} m')
    poles, tops, mapped = scatter(frame, ter, land, osm, roads, boundary)
    lap = cKDTree(line[:, :2])   # nothing solid within 3 m of the lane, where the AI runs wide
    poles = [p for p in poles if lap.query(p[:2])[0] > half + 3.0]
    tops = [t for t in tops if lap.query(t[:2])[0] > half + 3.0]
    log(f'{len(tops)} trees ({mapped} mapped), {sum(1 for p in poles if p[2] > 0)} lamps')

    # meshes per tile
    tiles = {}
    x0, y0 = ter.tx0, ter.ty0
    key = lambda x, y: (int((x - x0) // TILE), int((y - y0) // TILE))
    for b in buildings:
        c = np.vstack([s[1] for s in b[1]]).mean(0)
        tiles.setdefault(key(*c[:2]), {'b': [], 'r': []})['b'].append(b)
    for r in roads:
        c = r.xy[len(r.xy) // 2]
        tiles.setdefault(key(*c), {'b': [], 'r': []})['r'].append(r)
    deck_tiles = {}
    for line_d, zl, hf in decks:
        deck_tiles.setdefault(key(*line_d[len(line_d) // 2]), []).append((line_d, zl, hf))
    for k in deck_tiles:
        tiles.setdefault(k, {'b': [], 'r': []})
    bkey = key(*boundary.mean(0))
    tiles.setdefault(bkey, {'b': [], 'r': []})
    flags = []
    for (i, j), cell in sorted(tiles.items()):
        # B: what stands up and is seen from afar (buildings, the bridges' slabs and parapets, the boundary's barrier);
        # R: the flat road surfaces and their lines, which the game draws only near the car (where its ground is finest)
        solid = building_prims(cell['b']) if cell['b'] else []
        for line_d, zl, hf in deck_tiles.get((i, j), []):
            p, nn, di = deck_mesh(line_d, zl, hf)
            # the sides take the texture along the bridge and up, the flat faces (parapet tops, underside) from above:
            # along-and-up on a flat face is one texture row smeared along the whole bridge
            uv = np.where(np.abs(nn[:, 1:2]) > 0.5, p[:, [0, 2]], np.c_[p[:, 0] + p[:, 2], p[:, 1]]) / 4.0
            solid.append(('CitySidewalk', p, nn, uv, fix_winding(p, nn, di)))
        if (i, j) == bkey:
            p, nn, bi = barrier_mesh(boundary, ter)
            solid.append(('Barrier', p, nn, np.zeros((len(p), 2)), bi))
        if solid:
            if MESHES:
                glb.write(os.path.join(OUT, f'SM_Erl_B_{i}_{j}.glb'), f'SM_Erl_B_{i}_{j}', solid)
        flat_prims = []
        pts, idx, lines = [], [], []
        for r in cell['r']:
            p, k_idx = road_mesh(r, ter)
            idx += [len(pts) + q for q in k_idx]
            pts += [tuple(v) for v in p]
            if not r.oneway and r.lanes >= 2 and r.kind in ('primary', 'secondary', 'tertiary', 'trunk', 'unclassified', 'residential'):
                lines += dashes(r, ter)
        if pts:
            p = np.array(pts)
            up = np.tile((0, 1.0, 0), (len(p), 1))
            flat_prims.append(('CityAsphalt', p, up, p[:, [0, 2]] / 4.0, fix_winding(p, up, idx)))
        if lines:
            p = np.array([q for quad in lines for q in quad], 'f8')
            up = np.tile((0, 1.0, 0), (len(p), 1))
            li = []
            for k in range(len(lines)):
                b0 = 4 * k
                li += [b0, b0 + 2, b0 + 1, b0 + 1, b0 + 2, b0 + 3]
            flat_prims.append(('CityLine', p, up, np.zeros((len(p), 2)), fix_winding(p, up, li)))
        if flat_prims:
            if MESHES:
                glb.write(os.path.join(OUT, f'SM_Erl_R_{i}_{j}.glb'), f'SM_Erl_R_{i}_{j}', flat_prims)
        flags.append((i, j, int(bool(solid)), int(bool(flat_prims))))
    log(f'{sum(f[2] for f in flags)} building tiles, {sum(f[3] for f in flags)} road tiles')

    # what the game reads
    edges = []
    for ring in rings + [boundary]:
        edges += [(*ring[k], *ring[(k + 1) % len(ring)]) for k in range(len(ring))]
    main_lines = chain([simplify(np.c_[r.xy, r.z], 1.0) for r in roads if r.kind in MAIN])
    minor_lines = [simplify(r.xy, 3.0) for r in roads if r.kind not in MAIN and r.kind != 'service']
    deck_rows = []
    for line_d, zl, hf in decks:
        for k in range(len(line_d) - 1):
            deck_rows.append((*line_d[k], zl[k], *line_d[k + 1], zl[k + 1], hf + 0.3))
    palette = np.array([(*c, wet, sink, hard) for _, c, wet, sink, hard in LAND], 'f4')
    area = (*boundary.min(0), *boundary.max(0))
    write_bin(BIN, {
        'hdr': ('d', [ter.tx0, ter.ty0, STEP, ter.nx, ter.ny, frame.datum, frame.E0, frame.N0, TILE]),
        'height': ('b', encode_heights(ter.cm)),
        'land': ('b', encode_runs(land | (sink.astype(np.uint8) << 7))),   # the class, and 128 where the ground is sunk under a road
        'palette': ('f', palette),
        'area': ('d', area),
        'line': ('d', line),
        'half': ('d', [half]),
        'edges': ('f', np.array(edges)),
        'decks': ('f', np.array(deck_rows) if deck_rows else np.zeros((0, 7))),
        'roads': ('f', lines_with_breaks(main_lines, 3)),
        'map': ('f', lines_with_breaks(minor_lines, 2)),
        'poles': ('f', np.array(poles)),
        'tops': ('f', np.array(tops)),
        'tiles': ('i', np.array(flags)),
    })
    log(f'{BIN}: {os.path.getsize(BIN) / 1e6:.1f} MB; {len(edges)} wall edges, {len(deck_rows)} deck pieces')

    facade_texture(os.path.join(OUT, 'T_ErlWall.png'), (0.62, 0.60, 0.57), seed=10)
    noise_texture(os.path.join(OUT, 'T_ErlNoise.png'))
    specs = {f'ErlWall{k}': {'kind': 'wall', 'tex': 'T_ErlWall.png', 'tint': list(t)} for k, t in enumerate(WALL_TINTS)}
    specs.update(ErlRoof={'kind': 'pbr', 'base': 'roof_base.jpg', 'normal': 'roof_normal.jpg', 'roughmap': 'roof_rough.jpg'},
                 ErlRoofFlat={'kind': 'flat', 'color': glb.srgb(0x5a5a58), 'metal': 0.0, 'rough': 0.85})
    json.dump(specs, open(os.path.join(OUT, 'materials.json'), 'w'), indent=1)
    plan(os.path.join(OUT, 'plan.png'), ter, land, line, boundary)
    log('done')


if __name__ == '__main__':
    main()
