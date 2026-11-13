# Generates the arcade city: about 2 km of open streets and blocks, with buildings you can hit, parks, and a timed circuit marked through the middle.
#   FSLapUE\Plugins\HoMoLapPhysics\...\Private\CityTrack.inl   centreline, buildings, barriers for the physics and the game
#   data\city\SM_CityBig.glb, SM_CityBox.glb, SM_CityBarrier.glb, T_Facade.png, materials.json   for tools\import_ue.py
# Everything is procedural, so nothing depends on outside art. Same axes as track_to_ue.py: glTF (x, up, y) is Unreal (X = x, Y = y, Z = up).
import json
import os
import random
import sys

import numpy as np
from PIL import Image

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import glb
from track_to_ue import UP, flat, quad, ribbon

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
INL = os.path.join(ROOT, 'FSLapUE', 'Plugins', 'HoMoLapPhysics', 'Source', 'HoMoLapPhysics', 'Private', 'CityTrack.inl')
OUT = os.path.join(ROOT, 'data', 'city')

# street centre lines, metres; the circuit runs on the middle of the grid, and every street around it is open
CORE_X = [0, 105, 210, 320, 430, 545, 655]
CORE_Y = [0, 90, 185, 280, 375, 470]
X = [-1130, -950, -780, -620, -470, -340, -220, -110] + CORE_X + [770, 890, 1020, 1160, 1310, 1470, 1640, 1820]
Y = [-1070, -890, -720, -560, -430, -300, -190, -90] + CORE_Y + [565, 670, 790, 920, 1060, 1210, 1370, 1540]
ROUTE = [(0, 0), (4, 0), (4, 1), (6, 1), (6, 4), (5, 4), (5, 5), (2, 5), (2, 3), (3, 3), (3, 2), (1, 2), (1, 4), (0, 4)]   # indices into CORE_X and CORE_Y, driven in this order
STREET = 8.0      # half width of a street
SIDEWALK = 3.0
CORNER = 20.0     # corner radius of the driven line
LANE_HALF = 7.0   # half width of the marked lane, for the edge lines and Track.HALF


def fillet_polyline(points, radius):
    """Closed polygon with its corners replaced by arcs, sampled about every metre."""
    n = len(points)
    out = []
    for i in range(n):
        p0, p1, p2 = points[i - 1], points[i], points[(i + 1) % n]
        d1 = (p1 - p0) / np.linalg.norm(p1 - p0)
        d2 = (p2 - p1) / np.linalg.norm(p2 - p1)
        a, b = p1 - d1 * radius, p1 + d2 * radius
        turn = np.sign(d1[0] * d2[1] - d1[1] * d2[0])
        centre = a + np.array([-d1[1], d1[0]]) * radius * turn
        start = np.arctan2(*(a - centre)[::-1])
        sweep = np.arctan2(*(b - centre)[::-1]) - start
        sweep = (sweep + np.pi) % (2 * np.pi) - np.pi
        for t in np.linspace(0, 1, max(4, int(abs(sweep) * radius))):
            out.append(centre + radius * np.array([np.cos(start + sweep * t), np.sin(start + sweep * t)]))
    return np.array(out)


def resample_closed(points, spacing=1.0):
    loop = np.vstack([points, points[0]])
    seg = np.hypot(*np.diff(loop, axis=0).T)
    keep = np.r_[True, seg > 1e-6]
    loop = loop[keep]
    s = np.r_[0, np.cumsum(np.hypot(*np.diff(loop, axis=0).T))]
    count = int(round(s[-1] / spacing))
    t = np.linspace(0, s[-1], count, endpoint=False)
    return np.c_[np.interp(t, s, loop[:, 0]), np.interp(t, s, loop[:, 1])]


def centreline():
    corners = [np.array([CORE_X[i], CORE_Y[j]], float) for i, j in ROUTE]
    line = resample_closed(fillet_polyline(corners, CORNER))
    # the lap starts halfway along the first straight, with room for the grid behind it
    start = int(np.argmin(np.hypot(line[:, 0] - (CORE_X[0] + CORE_X[4]) / 2, line[:, 1] - CORE_Y[0])))
    return np.roll(line, -start, axis=0)


def corridors():
    """The fast roads, as polylines about every 4 m: an inner ring road, an outer one near the edge of the map,
    and four long diagonals from the corners of the centre out to the outer ring."""
    cx, cy = (CORE_X[0] + CORE_X[-1]) / 2, (CORE_Y[0] + CORE_Y[-1]) / 2
    t = np.linspace(0, 2 * np.pi, 2500, endpoint=False)
    c, s = np.cos(t), np.sin(t)
    half_x, half_y = min(cx - X[0], X[-1] - cx) * 0.88, min(cy - Y[0], Y[-1] - cy) * 0.88
    rings = [resample_closed(np.c_[cx + a * np.sign(c) * np.abs(c) ** 0.5, cy + b * np.sign(s) * np.abs(s) ** 0.5], 4.0) for a, b in ((860, 710), (half_x, half_y))]
    ring = rings[-1]
    angle = np.arctan2(ring[:, 1] - cy, ring[:, 0] - cx)
    diagonals = []
    i0, i1, j0, j1 = X.index(CORE_X[0]) - 1, X.index(CORE_X[-1]) + 1, Y.index(CORE_Y[0]) - 1, Y.index(CORE_Y[-1]) + 1
    for x, y in ((X[i0], Y[j0]), (X[i1], Y[j0]), (X[i0], Y[j1]), (X[i1], Y[j1])):
        hit = ring[np.argmin(np.abs(np.angle(np.exp(1j * (angle - np.arctan2(y - cy, x - cx))))))]
        count = int(np.hypot(hit[0] - x, hit[1] - y) / 4)
        diagonals.append(np.c_[np.linspace(x, hit[0], count), np.linspace(y, hit[1], count)])
    return rings, diagonals


def road_prims(line, half, closed):
    """Asphalt and two edge lines, laid above the sidewalks of the blocks the road cuts through."""
    d = (np.roll(line, -1, axis=0) - np.roll(line, 1, axis=0)) if closed else np.gradient(line, axis=0)
    length = np.hypot(*d.T)
    nx, ny = -d[:, 1] / length, d[:, 0] / length
    out = []
    for material, width, offset, z in (('CityAsphalt', half, 0.0, 0.04), ('CityLine', 0.12, half - 0.5, 0.05), ('CityLine', 0.12, 0.5 - half, 0.05)):
        *arrays, idx = ribbon(line[:, 0] + nx * offset, line[:, 1] + ny * offset, nx, ny, width, z, 4.0)
        out.append((material, *arrays, idx if closed else idx[:-6]))
    return out


def terrain_border(x0, y0, x1, y1, margin=1500.0):
    """Low-relief visual terrain outside the playable city; roads remain on the flat map plane."""
    def height(x, y):
        return 2.5 * np.sin(x / 260.0) + 1.8 * np.cos(y / 310.0) + 1.2 * np.sin((x + y) / 470.0)

    strips = [
        [(x0 - margin, y0 - margin), (x0, y0), (x0, y1), (x0 - margin, y1 - margin)],
        [(x1, y0), (x1 + margin, y0 - margin), (x1 + margin, y1 - margin), (x1, y1)],
        [(x0, y0 - margin), (x1, y0 - margin), (x1, y0), (x0, y0)],
        [(x0, y1), (x1, y1), (x1, y1 + margin), (x0, y1 + margin)],
    ]
    out = []
    for points in strips:
        p = np.array([(x, height(x, y), y) for x, y in points], 'f8')
        out.append(('CityFar', p, np.tile(UP, (4, 1)), np.zeros((4, 2)), [0, 1, 2, 0, 2, 3]))
    return out


def barriers():
    """A low wall across each street where it leaves the map: (x, y, half x, half y, height)."""
    span = STREET + SIDEWALK
    out = [(x, y, span, 0.5, 1.1) for x in X for y in (Y[0] - 95, Y[-1] + 95)]
    return out + [(x, y, 0.5, span, 1.1) for y in Y for x in (X[0] - 105, X[-1] + 105)]


def buildings(rng):
    xs = [X[0] - 110] + X + [X[-1] + 110]
    ys = [Y[0] - 100] + Y + [Y[-1] + 100]
    margin = STREET + SIDEWALK
    out, blocks, parks = [], [], []
    cx, cy = (CORE_X[0] + CORE_X[-1]) / 2, (CORE_Y[0] + CORE_Y[-1]) / 2
    for a in range(len(xs) - 1):
        for b in range(len(ys) - 1):
            x0, x1, y0, y1 = xs[a] + margin, xs[a + 1] - margin, ys[b] + margin, ys[b + 1] - margin
            blocks.append((x0, y0, x1, y1))
            off_x, off_y = (x0 + x1) / 2 - cx, (y0 + y1) / 2 - cy
            industrial, housing = off_x > 420 and off_y < 150, off_x < -380   # the districts: sheds to the east, houses to the west, offices elsewhere
            if rng.random() < (0.22 if housing else 0.1):
                parks.append((x0, y0, x1, y1))
                continue
            columns = 2 if x1 - x0 < 75 else 3
            rows = 2 if y1 - y0 < 70 else 3
            if industrial:
                columns, rows = 1, rng.choice((1, 2))
            elif housing:
                columns, rows = 4, 3
            alley = None if industrial or housing or rng.random() > 0.35 else columns // 2   # a lane through the block
            edges_x, edges_y = [x0], [y0]
            for k in range(1, columns):
                edges_x.append(x0 + (x1 - x0) * (k + rng.uniform(-0.12, 0.12)) / columns)
            for k in range(1, rows):
                edges_y.append(y0 + (y1 - y0) * (k + rng.uniform(-0.12, 0.12)) / rows)
            edges_x.append(x1)
            edges_y.append(y1)
            downtown = 1 - min(1.0, np.hypot(((x0 + x1) / 2 - cx) / 420, ((y0 + y1) / 2 - cy) / 330))
            suburb = min(1.0, np.hypot(((x0 + x1) / 2 - cx) / 1000, ((y0 + y1) / 2 - cy) / 850))
            for c in range(columns):
                for r in range(rows):
                    bx0, bx1, by0, by1 = edges_x[c], edges_x[c + 1], edges_y[r], edges_y[r + 1]
                    if alley is not None:
                        bx0 += 3.5 if c == alley else 0
                        bx1 -= 3.5 if c == alley - 1 else 0
                    height = rng.uniform(14, 34) * (1 - 0.6 * suburb) + downtown * rng.uniform(10, 70)
                    if industrial or housing:
                        height = rng.uniform(8, 14) if industrial else rng.uniform(5, 10)
                    if downtown > 0.5 and rng.random() < 0.15:
                        height *= 2.2   # a few skyscrapers
                    mx, my, hx, hy = (bx0 + bx1) / 2, (by0 + by1) / 2, (bx1 - bx0) / 2 - 0.3, (by1 - by0) / 2 - 0.3
                    out.append((mx, my, hx, hy, height))
                    if height > 40:     # tall buildings step back towards the top
                        top = rng.uniform(0.55, 0.8)
                        out.append((mx, my, hx * top, hy * top, height * rng.uniform(1.15, 1.4)))
    return out, blocks, parks


def props(rng, parks):
    """Street lamps and trees on the sidewalks of every street, and trees in the parks.
    Returns boxes (x, y, z, size x, size y, size z) for poles, arms and trunks, and spheres (x, y, z, radius) for tree tops."""
    segments = [((x, a), (x, b), (1.0, 0.0)) for x in X for a, b in zip(Y, Y[1:])]
    segments += [((a, y), (b, y), (0.0, 1.0)) for y in Y for a, b in zip(X, X[1:])]
    boxes, spheres = [], []
    offset = STREET + 1.3
    count = 0
    for p, q, n in segments:
        p, q, n = np.array(p, float), np.array(q, float), np.array(n)
        length = np.hypot(*(q - p))
        for s in np.arange(22, length - 21, 34):   # not in the junctions
            count += 1
            side = 1 if count % 2 == 0 else -1
            foot = p + (q - p) * s / length + n * side * offset
            if count % 4 < 2:   # a lamp: a pole with an arm reaching over the road
                boxes.append((foot[0], foot[1], 0, 0.2, 0.2, 8.5))
                arm = foot - n * side * 1.0
                boxes.append((arm[0], arm[1], 8.35, abs(n[0]) * 2.0 + 0.14, abs(n[1]) * 2.0 + 0.14, 0.14))
            else:               # a tree: a trunk and a round top
                boxes.append((foot[0], foot[1], 0, 0.34, 0.34, 4.2))
                spheres.append((foot[0], foot[1], 5.6, 2.4))
    for x0, y0, x1, y1 in parks:
        for x in np.arange(x0 + 8, x1 - 6, 18):
            for y in np.arange(y0 + 8, y1 - 6, 18):
                fx, fy, r = x + rng.uniform(-5, 5), y + rng.uniform(-5, 5), rng.uniform(2.4, 4.2)
                boxes.append((fx, fy, 0, 0.4, 0.4, r + 1.6))
                spheres.append((fx, fy, r + 3.2, r))
    return boxes, spheres


def street_dashes(every=10.0, length=3.5, width=0.09):
    """Quads down the middle of every street, between the junctions."""
    rects = [(x, y, width, length / 2) for x in X for a, b in zip(Y, Y[1:]) for y in np.arange(a + 20, b - 19, every)]
    rects += [(x, y, length / 2, width) for y in Y for a, b in zip(X, X[1:]) for x in np.arange(a + 20, b - 19, every)]
    return line_quads(rects)


def crossings(stripe=0.5, length=3.0, gap=1.6):
    """Zebra crossings on the four arms of every junction."""
    near = STREET + SIDEWALK + 2.0
    rects = []
    for x in X:
        for y in Y:
            for offset in np.arange(-STREET + 1, STREET - 0.5, gap):
                rects += [(x + offset, y + side * near, stripe / 2, length / 2) for side in (-1, 1)]
                rects += [(x + side * near, y + offset, length / 2, stripe / 2) for side in (-1, 1)]
    return line_quads(rects)


def line_quads(rects):
    points, idx = [], []
    for x, y, hx, hy in rects:
        base = len(points)
        points += [(x - hx, 0.03, y - hy), (x - hx, 0.03, y + hy), (x + hx, 0.03, y - hy), (x + hx, 0.03, y + hy)]
        idx += [base, base + 1, base + 2, base + 1, base + 3, base + 2]
    return 'CityLine', np.array(points, 'f8'), np.tile(UP, (len(points), 1)), np.zeros((len(points), 2)), idx


def edge_line(line, nx, ny, side):
    return ribbon(line[:, 0] + nx * side * LANE_HALF, line[:, 1] + ny * side * LANE_HALF, nx, ny, 0.08, 0.03, 1.0)


def unit_box(material):
    """1 m wide and deep, 1 m tall, standing on z = 0, with the top and four sides."""
    return (material,) + box_arrays()


def box_arrays():
    h = 0.5
    p, n, idx = [], [], []
    faces = [((-h, 1, -h), (-h, 1, h), (h, 1, -h), (h, 1, h), (0, 1, 0)),
             ((-h, 0, h), (h, 0, h), (-h, 1, h), (h, 1, h), (0, 0, 1)),
             ((h, 0, -h), (-h, 0, -h), (h, 1, -h), (-h, 1, -h), (0, 0, -1)),
             ((h, 0, h), (h, 0, -h), (h, 1, h), (h, 1, -h), (1, 0, 0)),
             ((-h, 0, -h), (-h, 0, h), (-h, 1, -h), (-h, 1, h), (-1, 0, 0))]
    for a, b, c, d, normal in faces:
        k = len(p)
        p += [a, b, c, d]
        n += [normal] * 4
        idx += [k, k + 1, k + 2, k + 1, k + 3, k + 2]
    return np.array(p, 'f8'), np.array(n, 'f8'), np.zeros((len(p), 2)), idx


def facade_texture(path, wall=(0.47, 0.455, 0.44), seed=4):
    """One bay of a building: 6 m wide, 7 m tall (two windows, two floors), a pale wall so the game can tint it."""
    size = 512
    rng = np.random.default_rng(seed)
    img = np.ones((size, size, 3), 'f4') * np.array(wall, 'f4')   # a mid tone by default: the walls take full sun
    img *= np.clip(rng.normal(1, 0.025, (size, size)), 0.8, 1.1)[..., None]
    columns = int(rng.integers(2, 4))   # 2 or 3 windows per 6 m bay, so the four wall types differ
    frame = np.array([0.30, 0.31, 0.33], 'f4')
    sill = np.clip(np.array(wall, 'f4') * 1.22, 0, 1)
    for row in range(2):
        slab = int(size * (row + 1) / 2)
        img[max(slab - 3, 0):slab] *= 0.82   # the floor slab's shadow line
        for col in range(columns):
            x0, x1 = int(size * (col + 0.22) / columns), int(size * (col + 0.78) / columns)
            y0, y1 = int(size * (row + 0.18) / 2), int(size * (row + 0.72) / 2)
            tone = rng.uniform(0.7, 1.2)
            glass = np.array([0.10, 0.16, 0.24], 'f4') * tone
            if rng.random() < 0.08:
                glass = np.array([0.5, 0.42, 0.26], 'f4')   # a lit room
            img[y0:y1, x0:x1] = glass
            img[y0 - 3:y0, x0 - 2:x1 + 2] = frame * 0.75           # lintel
            img[y1:y1 + 4, x0 - 4:x1 + 4] = sill                   # sill ledge
            img[y0:y1, x0:x0 + 2] = img[y0:y1, x1 - 2:x1] = frame  # window frame
            mid = (x0 + x1) // 2
            img[y0:y1, mid - 2:mid + 2] = frame
            if rng.random() < 0.5:
                bar = (y0 + y1) // 2
                img[bar - 1:bar + 1, x0:x1] = frame                # transom bar
    Image.fromarray((np.clip(img, 0, 1) * 255).astype('u1')).save(path)


def write_inl(line, bldg, bars, boxes, spheres, roads):
    def array(name, rows, kind='CityBox'):
        return f'const {kind} {name}[] = {{\n' + ',\n'.join('{ ' + ', '.join(f'{v:.3f}' for v in row) + ' }' for row in rows) + '\n};\n'
    with open(INL, 'w', newline='\n') as out:
        out.write('// Generated by tools\\city_to_ue.py. Do not edit.\n')
        out.write('const double CityX[] = { ' + ', '.join(f'{v:.3f}' for v in line[:, 0]) + ' };\n')
        out.write('const double CityY[] = { ' + ', '.join(f'{v:.3f}' for v in line[:, 1]) + ' };\n')
        out.write(f'const double CityLaneHalf = {LANE_HALF};\n')
        out.write('const double CityRoadX[] = { ' + ', '.join(f'{v:.1f}' for v in roads[:, 0]) + ' };\n')
        out.write('const double CityRoadY[] = { ' + ', '.join(f'{v:.1f}' for v in roads[:, 1]) + ' };\n')
        out.write('const double CityStreetX[] = { ' + ', '.join(str(v) for v in X) + ' };\n')
        out.write('const double CityStreetY[] = { ' + ', '.join(str(v) for v in Y) + ' };\n')
        out.write(array('CityBuildings', bldg))
        out.write(array('CityBarriers', bars))
        out.write(array('CityPoles', boxes, 'CityPole'))
        out.write(array('CityTreeTops', spheres, 'CitySphere'))


def main():
    rng = random.Random(49)
    line = centreline()
    bars = barriers()
    bldg, blocks, parks = buildings(rng)
    boxes, spheres = props(rng, parks)
    rings, diagonals = corridors()
    fast = np.vstack(rings + diagonals)

    def clear(x, y, margin):
        return np.hypot(fast[:, 0] - x, fast[:, 1] - y).min() > margin
    bldg = [b for b in bldg if clear(b[0], b[1], 13 + np.hypot(b[2], b[3]))]
    boxes = [b for b in boxes if clear(b[0], b[1], 13)]
    spheres = [s for s in spheres if clear(s[0], s[1], 15)]
    gap = np.array([[1e9, 1e9]])   # breaks the line on the map
    pieces = [part for r in rings for part in (gap, r[::8], r[:1])] + [part for d in diagonals for part in (gap, d[::16], d[-1:])]
    roads = np.vstack(pieces[1:])
    write_inl(line, bldg, bars, boxes, spheres, roads)

    os.makedirs(OUT, exist_ok=True)
    d = np.roll(line, -1, axis=0) - np.roll(line, 1, axis=0)
    length = np.hypot(*d.T)
    nx, ny = -d[:, 1] / length, d[:, 0] / length
    xs0, ys0 = X[0] - 120, Y[0] - 110
    xs1, ys1 = X[-1] + 120, Y[-1] + 110
    prims = terrain_border(xs0, ys0, xs1, ys1)
    prims.append(('CityAsphalt', *quad(xs0, ys0, xs1, ys1, 0.0, 4.0)))
    for x0, y0, x1, y1 in blocks:
        prims.append(('CitySidewalk', *quad(x0 - SIDEWALK, y0 - SIDEWALK, x1 + SIDEWALK, y1 + SIDEWALK, 0.02, 2.5)))
    for x0, y0, x1, y1 in parks:
        prims.append(('Grass', *quad(x0, y0, x1, y1, 0.03, 8.0)))
    prims.append(street_dashes())
    prims.append(crossings())
    for ring, half in zip(rings, (12.0, 13.0)):
        prims += road_prims(ring, half, True)
    for diagonal in diagonals:
        prims += road_prims(diagonal, 9.0, False)
    prims.append(('CityLine', *edge_line(line, nx, ny, 1)))
    prims.append(('CityLine', *edge_line(line, nx, ny, -1)))
    # the start line, across the lane at s = 0
    t = np.array([d[0, 0], d[0, 1]]) / length[0]
    n = np.array([nx[0], ny[0]])
    c = line[0]
    pts = [c - t * 0.25 - n * LANE_HALF, c - t * 0.25 + n * LANE_HALF, c + t * 0.25 - n * LANE_HALF, c + t * 0.25 + n * LANE_HALF]
    prims.append(('Line', np.array([(p[0], 0.035, p[1]) for p in pts], 'f8'), np.tile(UP, (4, 1)), np.zeros((4, 2)), [0, 1, 2, 1, 3, 2]))
    glb.write(os.path.join(OUT, 'SM_CityBig.glb'), 'SM_CityBig', prims)
    glb.write(os.path.join(OUT, 'SM_CityBox.glb'), 'SM_CityBox', [unit_box('Facade')])
    glb.write(os.path.join(OUT, 'SM_CityBarrier.glb'), 'SM_CityBarrier', [unit_box('Barrier')])
    glb.write(os.path.join(OUT, 'SM_CityPole.glb'), 'SM_CityPole', [unit_box('CityPole')])
    facade_texture(os.path.join(OUT, 'T_Facade.png'))

    def ground(color, rough=0.9):
        return {'kind': 'ground', 'color': glb.srgb(color), 'metal': 0.0, 'rough': rough}
    json.dump({'CityFar': ground(0x2f3338), 'CityAsphalt': ground(0x3e4145), 'CitySidewalk': ground(0x8d8a84, 0.85),
               'CityLine': {'kind': 'flat', 'color': glb.srgb(0xe8e6df), 'metal': 0.0, 'rough': 0.6},
               'Facade': {'kind': 'facade', 'tex': 'T_Facade.png'},
               'CityPole': {'kind': 'flat', 'color': glb.srgb(0x34383d), 'metal': 0.3, 'rough': 0.55},
               'CityLeaf': {'kind': 'flat', 'color': glb.srgb(0x3f6f35), 'metal': 0.0, 'rough': 0.85},
               'Barrier': {'kind': 'flat', 'color': glb.srgb(0xc8362f), 'metal': 0.0, 'rough': 0.7}},
              open(os.path.join(OUT, 'materials.json'), 'w'), indent=1)

    # a quick plan view, so the layout can be checked without Unreal
    scale = 0.4
    w, h = int((xs1 - xs0) * scale), int((ys1 - ys0) * scale)
    plan = np.full((h, w, 3), 60, 'u1')
    def to_px(x, y):
        return int((x - xs0) * scale), int((y - ys0) * scale)
    for x, y, hx, hy, _ in bldg:
        a, b = to_px(x - hx, y - hy), to_px(x + hx, y + hy)
        plan[b[1] - 1 if b[1] <= a[1] else a[1]:b[1], a[0]:b[0]] = (130, 130, 150)
    for x, y, hx, hy, _ in bars:
        a, b = to_px(x - hx, y - hy), to_px(x + hx, y + hy)
        plan[a[1]:max(b[1], a[1] + 2), a[0]:max(b[0], a[0] + 2)] = (230, 60, 50)
    for x, y in fast[::2]:
        px, py = to_px(x, y)
        plan[py - 2:py + 3, px - 2:px + 3] = (40, 40, 44)
    for k, (x, y) in enumerate(line):
        px, py = to_px(x, y)
        plan[py - 1:py + 2, px - 1:px + 2] = (255, 220, 60) if k > 40 else (60, 255, 90)
    Image.fromarray(plan).save(os.path.join(OUT, 'plan.png'))

    closest = min(min(abs(lx - bx) - hx if abs(ly - by) <= hy else 1e9 for bx, by, hx, hy, _ in bldg) for lx, ly in line[::2])
    print(f'city: {len(boxes)} pole pieces, {len(spheres)} trees')
    print(f'city: lap {len(line)} m, {len(bldg)} buildings, {len(bars)} barriers, nearest building face to the line {closest:.1f} m')


if __name__ == '__main__':
    main()
