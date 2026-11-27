# Generates the track for Unreal from HoMo-LAP's centreline (E:\HOT LAP\source\fsg_track.json):
#   FSLapUE\Plugins\HoMoLapPhysics\...\Private\FSGTrack.inl   centreline for the C++ physics (rebuild the game after a change)
#   data\track\ground.glb, cone_*.glb, noise.png, materials.json   for tools\import_ue.py
# The FSG track is mirrored back and turned as the real site lies (fsg_line). The other events' tracks
# (vehicle-data\tracks\<id>.json, from tools\logs\track_from_log.py) go to ...\Private\EventTracks.inl and
# data\track\ground_<id>.glb. Import the grounds alone with FSLAP_IMPORT=tracks.
# glTF axes here are (x, up, y) with the track's x and y, which Unreal imports as X = x, Y = y, Z = up.
import json
import os
import sys

import numpy as np
from PIL import Image

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import glb

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
SOURCE = os.path.join(os.path.dirname(ROOT), 'source', 'fsg_track.json')
INL = os.path.join(ROOT, 'FSLapUE', 'Plugins', 'HoMoLapPhysics', 'Source', 'HoMoLapPhysics', 'Private', 'FSGTrack.inl')
INL_EVENTS = os.path.join(os.path.dirname(INL), 'EventTracks.inl')
EVENTS = [('fscz', 'FS Czech'), ('fsf', 'FS France')]   # the order of the menu's tracks after FSG
FSG_TURN_DEG = -156.6   # fsg_line
FSG_START_M = 632.0
OUT = os.path.join(ROOT, 'data', 'track')
HALF = 2.0   # same as Track.HALF in HoMoLapCore
UP = (0.0, 1.0, 0.0)


def flat(pts, tile):
    pts = np.array(pts, 'f8')
    return pts, np.tile(UP, (len(pts), 1)), pts[:, [0, 2]] / tile


def quad(x0, y0, x1, y1, z, tile):
    return *flat([(x0, z, y0), (x0, z, y1), (x1, z, y0), (x1, z, y1)], tile), [0, 1, 2, 1, 3, 2]


def lathe(profile, material, segments=16):
    """Surface of revolution around the up axis; profile is [(radius, height), ...], bottom to top."""
    rings = len(profile)
    a = np.linspace(0, 2 * np.pi, segments + 1)
    r, h = np.array(profile).T
    slope = np.gradient(r, h)
    p = np.array([[ri * np.cos(t), hi, ri * np.sin(t)] for ri, hi in zip(r, h) for t in a])
    n = np.array([[np.cos(t), -s, np.sin(t)] for s in slope for t in a])
    n /= np.linalg.norm(n, axis=1, keepdims=True)
    idx = []
    for i in range(rings - 1):
        for j in range(segments):
            a0, a1 = i * (segments + 1) + j, i * (segments + 1) + j + 1
            b0, b1 = a0 + segments + 1, a1 + segments + 1
            idx += [a0, b0, a1, a1, b0, b1]
    return material, p, n, np.zeros((len(p), 2)), idx


def box_top(size, height, material):
    s = size / 2
    p, n, idx = [], [], []
    faces = [((-s, height, -s), (-s, height, s), (s, height, -s), (s, height, s), (0, 1, 0)),
             ((-s, 0, s), (s, 0, s), (-s, height, s), (s, height, s), (0, 0, 1)),
             ((s, 0, -s), (-s, 0, -s), (s, height, -s), (-s, height, -s), (0, 0, -1)),
             ((s, 0, s), (s, 0, -s), (s, height, s), (s, height, -s), (1, 0, 0)),
             ((-s, 0, -s), (-s, 0, s), (-s, height, -s), (-s, height, s), (-1, 0, 0))]
    for a, b, c, d, normal in faces:
        k = len(p)
        p += [a, b, c, d]
        n += [normal] * 4
        idx += [k, k + 1, k + 2, k + 1, k + 3, k + 2]
    return material, np.array(p, 'f8'), np.array(n, 'f8'), np.zeros((len(p), 2)), idx


class Terrain:
    """a track's heights (vehicle-data\\tracks\\<id>.json "terrain"): bilinear on its grid, its edge beyond it"""
    def __init__(self, t):
        self.x0, self.y0, self.step, self.nx, self.ny = t['x0'], t['y0'], t['step'], t['nx'], t['ny']
        self.z = np.array(t['z'], float).reshape(self.ny, self.nx)

    def at(self, x, y):
        from scipy import ndimage as nd
        c = np.clip((np.asarray(x) - self.x0) / self.step, 0, self.nx - 1)
        r = np.clip((np.asarray(y) - self.y0) / self.step, 0, self.ny - 1)
        return nd.map_coordinates(self.z, [r, c], order=1)


def terrain_mesh(ter, apron):
    """the terrain grid as two meshes, apron (inside the apron box) and grass, and a skirt 400 m out at the edge's height"""
    gx = np.r_[ter.x0 - 400, ter.x0 + np.arange(ter.nx) * ter.step, ter.x0 + (ter.nx - 1) * ter.step + 400]
    gy = np.r_[ter.y0 - 400, ter.y0 + np.arange(ter.ny) * ter.step, ter.y0 + (ter.ny - 1) * ter.step + 400]
    Z = np.pad(ter.z, 1, mode='edge') - 0.02
    GX, GY = np.meshgrid(gx, gy)
    pts = np.c_[GX.ravel(), Z.ravel(), GY.ravel()]
    dzx = np.gradient(Z, gx, axis=1)   # normals from the height's slope
    dzy = np.gradient(Z, gy, axis=0)
    nrm = np.c_[-dzx.ravel(), np.ones(Z.size), -dzy.ravel()]
    nrm /= np.linalg.norm(nrm, axis=1, keepdims=True)
    nx_ = len(gx)
    a = (np.arange(len(gy) - 1)[:, None] * nx_ + np.arange(nx_ - 1)[None, :]).ravel()
    cx = ((GX[:-1, :-1] + GX[1:, 1:]) / 2).ravel()
    cy = ((GY[:-1, :-1] + GY[1:, 1:]) / 2).ravel()
    inside = (cx > apron[0]) & (cx < apron[2]) & (cy > apron[1]) & (cy < apron[3])

    def tris(sel):
        q = a[sel]
        return np.c_[q, q + nx_, q + 1, q + 1, q + nx_, q + nx_ + 1].ravel()
    return [('Grass', pts, nrm, pts[:, [0, 2]] / 8.0, tris(~inside)), ('Apron', pts, nrm, pts[:, [0, 2]] / 4.0, tris(inside))]


def write_ground(path, name, x, y, ter=None):
    """grass, the apron around the track and the line across the track at s = 0; with a terrain the ground follows its
    heights. No road strip: as at a real event the course is open tarmac marked only by its cones (the team event's
    drivers, 2026-11-21: "the tracks does not have those cones lying in the way. instead the road itself changes the path")"""
    dx, dy = np.roll(x, -1) - np.roll(x, 1), np.roll(y, -1) - np.roll(y, 1)
    length = np.hypot(dx, dy)
    nx, ny = -dy / length, dx / length
    x0, y0, x1, y1 = x.min() - 14, y.min() - 14, x.max() + 14, y.max() + 14
    cx, cy = (x0 + x1) / 2, (y0 + y1) / 2
    tx, ty = x[0], y[0]
    start = (tx - dx[0] / length[0] * 0.15, ty - dy[0] / length[0] * 0.15, tx + dx[0] / length[0] * 0.15, ty + dy[0] / length[0] * 0.15)
    sn = (nx[0] * (HALF + 0.5), ny[0] * (HALF + 0.5))
    line = np.array([(start[0] - sn[0], 0.015, start[1] - sn[1]), (start[0] + sn[0], 0.015, start[1] + sn[1]),
                     (start[2] - sn[0], 0.015, start[3] - sn[1]), (start[2] + sn[0], 0.015, start[3] + sn[1])])
    if ter is None:
        glb.write(path, name, [
            ('Grass', *quad(cx - 700, cy - 700, cx + 700, cy + 700, -0.05, 8.0)),
            ('Apron', *quad(x0, y0, x1, y1, 0.0, 4.0)),
            ('Line', line, np.tile(UP, (4, 1)), np.zeros((4, 2)), [0, 1, 2, 1, 3, 2]),
        ])
        return
    line[:, 1] = ter.at(line[:, 0], line[:, 2]) + 0.045
    glb.write(path, name, terrain_mesh(ter, (x0, y0, x1, y1)) + [
        ('Line', line, np.tile(UP, (4, 1)), np.zeros((4, 2)), [0, 1, 2, 1, 3, 2]),
    ])


def fsg_line(track):
    """the LTS track as it really lies: its y had the wrong sign (the owner, 2026-11-16: "the map from LTS was mistaken
    as mirrored due to wrong sign"), so it is mirrored back. Turned as the car's log shows the site (the start line at the
    bottom right), and started at the real start line, 632 m into the LTS lap (both found by fitting the LTS line onto the
    line rebuilt from EnduFSG26.asc, rms 15 m; tools\\logs\\track_from_log.py fsg)"""
    x, y = np.array(track['x'], float), -np.array(track['y'], float)
    a = np.radians(FSG_TURN_DEG)
    x, y = x * np.cos(a) - y * np.sin(a), x * np.sin(a) + y * np.cos(a)
    k = int(round(FSG_START_M))   # the points are 1 m apart
    x, y = np.roll(x, -k), np.roll(y, -k)
    return x - x.min() + 40, y - y.min() + 40, k / len(x)


def write_events():
    """the other events' tracks (vehicle-data\\tracks\\<id>.json, from tools\\logs\\track_from_log.py): centreline, reference lap, ground"""
    lines = ["// Generated by tools\\track_to_ue.py from vehicle-data\\tracks\\*.json. Do not edit.",
             "struct EventTrackData { const char* id; const char* name; const char* refName; const double* x; const double* y; int n;",
             "\tconst double* s; const double* t; int nRef; double refLength, refLap, split1, split2;",
             "\tconst float* terrain; int tnx, tny; double tx0, ty0, tstep; };   // heights (m) on a grid, rows along y; none: flat"]
    rows = []
    for i, (key, _) in enumerate(EVENTS):
        track = json.load(open(os.path.join(ROOT, 'vehicle-data', 'tracks', key + '.json')))
        ref = track['ref']
        for name, values in ((f'Ev{i}X', track['x']), (f'Ev{i}Y', track['y']), (f'Ev{i}S', ref['s']), (f'Ev{i}T', ref['t'])):
            lines.append(f'const double {name}[] = {{ ' + ', '.join(repr(float(v)) for v in values) + ' };')
        ter = track.get('terrain')
        if ter:
            lines.append(f'const float Ev{i}G[] = {{ ' + ', '.join(f'{float(v):.2f}f' for v in ter['z']) + ' };')
            grid = f'Ev{i}G, {ter["nx"]}, {ter["ny"]}, {float(ter["x0"])!r}, {float(ter["y0"])!r}, {float(ter["step"])!r}'
        else:
            grid = 'nullptr, 0, 0, 0.0, 0.0, 1.0'
        rows.append(f'\t{{ "{key}", "{track["name"]}", "{ref["name"]}", Ev{i}X, Ev{i}Y, {len(track["x"])}, Ev{i}S, Ev{i}T, {len(ref["s"])}, '
                    f'{float(ref["length"])!r}, {float(ref["lapTime"])!r}, {float(ref["splits"][0])!r}, {float(ref["splits"][1])!r}, {grid} }},')
        write_ground(os.path.join(OUT, f'ground_{key}.glb'), f'SM_Ground_{key}', np.array(track['x']), np.array(track['y']),
                     Terrain(ter) if ter else None)
        print(f'{track["name"]} track written: {len(track["x"])} points')
    lines += ["const EventTrackData EventTracks[] = {"] + rows + ["};"]
    with open(INL_EVENTS, 'w', newline='\n') as out:
        out.write('\n'.join(lines) + '\n')


def main():
    os.makedirs(OUT, exist_ok=True)
    write_events()
    track = json.load(open(SOURCE))
    fx, fy, shift = fsg_line(track)
    with open(INL, 'w', newline='\n') as out:
        out.write('// Generated by tools\\track_to_ue.py from source\\fsg_track.json, mirrored and turned (fsg_line). Do not edit.\n')
        for name, values in (('FsgX', fx), ('FsgY', fy)):
            out.write(f'const double {name}[] = {{ ' + ', '.join(repr(round(float(v), 3)) for v in values) + ' };\n')
        lts = track['lts']
        for name, key in (('LtsS', 's'), ('LtsT', 't')):
            out.write(f'const double {name}[] = {{ ' + ', '.join(repr(float(v)) for v in lts[key]) + ' };\n')
        out.write(f"const double LtsLength = {float(lts['length'])!r}, LtsLap = {float(lts['lapTime'])!r}, LtsSplit1 = {float(lts['splits'][0])!r}, LtsSplit2 = {float(lts['splits'][1])!r};\n")
        out.write(f"const double LtsShift = {shift!r};   // the real start line lies this share of a lap into the LTS lap\n")

    x, y = fx, fy
    x0, y0, x1, y1 = x.min() - 14, y.min() - 14, x.max() + 14, y.max() + 14
    os.makedirs(OUT, exist_ok=True)
    write_ground(os.path.join(OUT, 'ground.glb'), 'SM_Ground', x, y)

    # free roam: paved paddock, start and finish of the acceleration strip, skidpad centre line (web game)
    acc_x, acc_y, acc_len, acc_w = x0 + 25, y0 - 50, 75, 5
    skid_x, skid_y, skid_d, skid_ri = x0 - 55, (y0 + y1) / 2, 18.25, 15.25 / 2
    def line_across(lx, ly, half):
        return ('Line', *quad(lx - 0.15, ly - half, lx + 0.15, ly + half, -0.012, 1.0))
    glb.write(os.path.join(OUT, 'paddock.glb'), 'SM_Paddock', [
        ('Apron', *quad(x0 - 250, y0 - 200, x1 + 200, y1 + 120, -0.02, 4.0)),
        line_across(acc_x, acc_y, acc_w / 2 + 0.3),
        line_across(acc_x + acc_len, acc_y, acc_w / 2 + 0.3),
        line_across(skid_x, skid_y, (skid_d - 2 * skid_ri + 0.6) / 2),
    ])

    # one piece of skid mark: 1 m square, reused and scaled by the game
    glb.write(os.path.join(OUT, 'skid.glb'), 'SM_Skid', [('Skid', *quad(-0.5, -0.5, 0.5, 0.5, 0.0, 1.0))])

    # 228 mm base, 325 mm tall (web game)
    def radius(h):
        return 0.1 * (1 - (h - 0.03) / 0.3)

    for key, body, stripe in (('b', 'ConeBlue', 'White'), ('y', 'ConeYellow', 'Black'), ('o', 'ConeOrange', 'White')):
        low, high = 0.1395, 0.1945
        glb.write(os.path.join(OUT, f'cone_{key}.glb'), f'SM_Cone_{key}', [
            lathe([(radius(0.03), 0.03), (radius(low), low)], body),
            lathe([(radius(low), low), (radius(high), high)], stripe),
            lathe([(radius(high), high), (0.0005, 0.33)], body),
            box_top(0.228, 0.03, 'ConeBase'),
        ])

    rng = np.random.default_rng(9)
    coarse = rng.random((32, 32))
    coarse = np.repeat(np.repeat(coarse, 8, axis=0), 8, axis=1)
    fine = rng.normal(0.0, 0.06, (256, 256))
    noise = np.clip(0.5 + (coarse - 0.5) * 0.22 + fine, 0, 1)
    Image.fromarray((noise * 255).astype('u1')).save(os.path.join(OUT, 'noise.png'))

    def ground(color):
        return {'kind': 'ground', 'color': glb.srgb(color), 'metal': 0.0, 'rough': 0.9}

    def plain(color, rough=0.6):
        return {'kind': 'flat', 'color': glb.srgb(color), 'metal': 0.0, 'rough': rough}

    json.dump({'Grass': ground(0x3f7339), 'Apron': ground(0x5b5e63), 'Road': ground(0x46494d), 'Line': plain(0xf3f1ea),
               'ConeBlue': plain(0x2f6fd6), 'ConeYellow': plain(0xf5c518), 'ConeOrange': plain(0xff7a1a),
               'White': plain(0xf3f1ea), 'Black': plain(0x1b1d20), 'ConeBase': plain(0x1e2023), 'Skid': plain(0x17181b, 0.85)},
              open(os.path.join(OUT, 'materials.json'), 'w'), indent=1)
    print('track written:', len(x), 'points')


if __name__ == '__main__':
    main()
