# Splits data\car\car_full.glb (tools\export_ue.py) into a body and four wheels with their pivots at the origin, and bakes
# the projected livery into the UV sheets 1001-1004 with the same maths as the web game's shader (template.html, liveryMaterial).
# Output in data\car: SM_Body.glb, SM_Wheel_*.glb, T_Livery_100x.png, material textures, materials.json. No Blender needed.
import json
import os
import re
import sys

import numpy as np
from PIL import Image
from scipy import ndimage

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import glb

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
CAR = os.path.join(ROOT, 'data', 'car')
LIVERY = os.path.join(os.path.dirname(ROOT), 'blender')   # HoMo-LAP's copy, what the web game shows
SIZE = 4096
MODEL_X0 = (1.702 + 3.232) / 2
CENTRE, HALF, SHARP = np.array([2.725, 0, 0.55]), np.array([1.525, 0.8, 0.55]), 8.0
LIVERY_BASE = {'Alu': (0xb8bcc2, 0.85, 0.35), 'Black': (0x0b0b0c, 0.1, 0.5), 'Carbon': (0x111315, 0.1, 0.5)}


def node_matrix(n):
    if 'matrix' in n:
        return np.array(n['matrix']).reshape(4, 4).T
    t = np.eye(4)
    t[:3, 3] = n.get('translation', [0, 0, 0])
    r = np.eye(4)
    if 'rotation' in n:
        x, y, z, w = n['rotation']
        r[:3, :3] = [[1 - 2 * (y * y + z * z), 2 * (x * y - z * w), 2 * (x * z + y * w)],
                     [2 * (x * y + z * w), 1 - 2 * (x * x + z * z), 2 * (y * z - x * w)],
                     [2 * (x * z - y * w), 2 * (y * z + x * w), 1 - 2 * (x * x + y * y)]]
    return t @ r @ np.diag([*n.get('scale', [1, 1, 1]), 1])


def wheel_name(pivot):
    return 'SM_Wheel_' + ('F' if pivot[0] > MODEL_X0 else 'R') + ('R' if pivot[2] > 0 else 'L')   # glTF +z is the car's right


def safe(name):
    return re.sub(r'[^A-Za-z0-9]', '', name.title())


def rasterise(uv, pos, nrm, P, N, mask):
    """Writes interpolated position and normal for every texel centre inside the triangles; uv is (T, 3, 2) in pixels."""
    h, w = mask.shape
    x0, x1 = np.floor(uv[:, :, 0].min(1)).astype(int).clip(0, w), np.ceil(uv[:, :, 0].max(1)).astype(int).clip(0, w)
    y0, y1 = np.floor(uv[:, :, 1].min(1)).astype(int).clip(0, h), np.ceil(uv[:, :, 1].max(1)).astype(int).clip(0, h)
    ax, ay, bx, by, cx, cy = uv[:, 0, 0], uv[:, 0, 1], uv[:, 1, 0], uv[:, 1, 1], uv[:, 2, 0], uv[:, 2, 1]
    det = (bx - ax) * (cy - ay) - (cx - ax) * (by - ay)
    for t in np.nonzero(np.abs(det) > 1e-9)[0]:
        ys, xs = np.mgrid[y0[t]:y1[t], x0[t]:x1[t]]
        px, py = xs + 0.5 - ax[t], ys + 0.5 - ay[t]
        w1 = (px * (cy[t] - ay[t]) - py * (cx[t] - ax[t])) / det[t]
        w2 = (py * (bx[t] - ax[t]) - px * (by[t] - ay[t])) / det[t]
        w0 = 1 - w1 - w2
        inside = (w0 >= 0) & (w1 >= 0) & (w2 >= 0)
        if inside.any():
            bary = np.stack([w0[inside], w1[inside], w2[inside]], 1)
            P[ys[inside], xs[inside]] = bary @ pos[t]
            N[ys[inside], xs[inside]] = bary @ nrm[t]
            mask[ys[inside], xs[inside]] = True


def sample(img, u, v):
    h, w = img.shape[:2]
    rows, cols = (1 - np.clip(v, 0, 1)) * h - 0.5, np.clip(u, 0, 1) * w - 0.5
    c = np.stack([ndimage.map_coordinates(img[..., k], [rows, cols], order=1, mode='nearest') for k in range(4)], 1)
    c[~((u >= 0) & (u <= 1) & (v >= 0) & (v <= 1))] = 0
    return c


def bake(tile, soup, views):
    P, N = np.zeros((SIZE, SIZE, 3), 'f4'), np.zeros((SIZE, SIZE, 3), 'f4')
    mask = np.zeros((SIZE, SIZE), bool)
    uv = np.concatenate([s[0] for s in soup]).astype('f8')
    uv[:, :, 0] = (uv[:, :, 0] - tile) * SIZE
    uv[:, :, 1] *= SIZE
    rasterise(uv, np.concatenate([s[1] for s in soup]), np.concatenate([s[2] for s in soup]), P, N, mask)

    p, n = P[mask], N[mask]
    n /= np.maximum(np.linalg.norm(n, axis=1, keepdims=True), 1e-9)
    q = 0.5 * (p - CENTRE) / HALF + 0.5
    def weight(d):
        return np.maximum(d, 0) ** SHARP
    layers = [('Left', 1 - q[:, 0], q[:, 2], weight(n[:, 1])), ('Right', q[:, 0], q[:, 2], weight(-n[:, 1])), ('Top', q[:, 0], q[:, 1], weight(n[:, 2])),
              ('Front', q[:, 1], q[:, 2], weight(n[:, 0])), ('Rear', 1 - q[:, 1], q[:, 2], weight(-n[:, 0]))]
    w_sum = weight(-n[:, 2])
    a_sum, rgb = np.zeros(len(p)), np.zeros((len(p), 3))
    for name, u, v, w in layers:
        c = sample(views[name], u, v)
        a = c[:, 3] * w
        a_sum += a
        w_sum += w
        rgb += c[:, :3] * a[:, None]
    alpha = np.clip(a_sum / np.maximum(w_sum, 1e-6), 0, 1)
    rgb = rgb / np.maximum(a_sum, 1e-6)[:, None]

    out = np.zeros((SIZE, SIZE, 4), 'f4')
    out[mask] = np.c_[rgb, alpha]
    # padding, so mip maps and filtering don't pull in unused texels: alpha from the nearest panel texel, colour from the nearest painted one
    painted = out[..., 3] > 1e-3
    near = ndimage.distance_transform_edt(~mask, return_distances=False, return_indices=True)
    out[..., 3] = out[..., 3][near[0], near[1]]
    if painted.any():
        near = ndimage.distance_transform_edt(~painted, return_distances=False, return_indices=True)
        out[..., :3] = out[..., :3][near[0], near[1]]
    name = f'T_Livery{1001 + tile}.png'   # no separator before the number: Unreal would merge T_Livery_1001.. into one UDIM texture
    Image.fromarray((out * 255 + 0.5).astype('u1'), 'RGBA').save(os.path.join(CAR, name))
    print(name, 'panel texels', int(mask.sum()), 'painted', int(painted.sum()), flush=True)
    return name


def main():
    gltf, blob = glb.read(os.path.join(CAR, 'car_full.glb'))
    uv_index = json.load(open(os.path.join(CAR, 'export_info.json')))['livery_uv_index']
    nodes, gmat = gltf['nodes'], gltf['materials']
    items = []

    def walk(i, parent, pivot):
        n = nodes[i]
        world = parent @ node_matrix(n)
        if pivot is None and n.get('name', '').startswith('S_S000_TIRE'):
            pivot = world[:3, 3].copy()
        if 'mesh' in n:
            items.append((n.get('name', ''), world, n['mesh'], pivot))
        for c in n.get('children', []):
            walk(c, world, pivot)

    for r in gltf['scenes'][gltf.get('scene', 0)]['nodes']:
        walk(r, np.eye(4), None)

    meshes, soups, materials, no_uv = {}, {}, {}, set()

    def describe(index):
        """Same rules as buildRealCar in the web game; returns the output material name for non-livery surfaces."""
        m = gmat[index] if index is not None else {}
        name = m.get('name', '')
        pbr = m.get('pbrMetallicRoughness', {})
        if re.search('tyre|tire|rubber', name, re.I):
            out, spec = 'Rubber', {'kind': 'flat', 'color': glb.srgb(0x121212), 'metal': 0.0, 'rough': 0.9}
        elif not name or re.search('carbon|fibre|fiber', name, re.I):
            out, spec = 'Carbon', {'kind': 'flat', 'color': glb.srgb(0x1e2125), 'metal': 0.35, 'rough': 0.38}
        else:
            out = safe(name)
            spec = {'kind': 'flat', 'color': pbr.get('baseColorFactor', [1, 1, 1, 1])[:3], 'metal': pbr.get('metallicFactor', 1.0), 'rough': pbr.get('roughnessFactor', 1.0)}
            if 'baseColorTexture' in pbr:
                image = gltf['images'][gltf['textures'][pbr['baseColorTexture']['index']]['source']]
                view = gltf['bufferViews'][image['bufferView']]
                file = f'T_{out}.' + image['mimeType'].split('/')[1]
                with open(os.path.join(CAR, file), 'wb') as f:
                    f.write(blob[view.get('byteOffset', 0):view.get('byteOffset', 0) + view['byteLength']])
                spec.update(kind='tex', tex=file, masked=m.get('alphaMode') == 'BLEND')
        materials[out] = spec
        return out

    def add(mesh, material, p, n, uv, idx):
        meshes.setdefault(mesh, {}).setdefault(material, []).append((p, n, uv, idx))

    for node_name, world, mesh_index, pivot in items:
        target = 'SM_Body' if pivot is None else wheel_name(pivot)
        origin = np.array([MODEL_X0, 0, 0]) if pivot is None else pivot
        linear = world[:3, :3]
        flip = np.linalg.det(linear) < 0
        normal_matrix = np.linalg.inv(linear).T
        for prim in gltf['meshes'][mesh_index]['primitives']:
            a = prim['attributes']
            v = (linear @ glb.accessor(gltf, blob, a['POSITION']).T).T + world[:3, 3]
            n = (normal_matrix @ glb.accessor(gltf, blob, a['NORMAL']).T).T
            n /= np.maximum(np.linalg.norm(n, axis=1, keepdims=True), 1e-12)
            tri = glb.accessor(gltf, blob, prim['indices']).astype('i8').reshape(-1, 3)
            if flip:
                tri = tri[:, [0, 2, 1]]
            index = prim.get('material')
            name = gmat[index].get('name', '') if index is not None else ''
            layer = uv_index.get(node_name)
            if '[LIVERY]' in name and layer is not None and f'TEXCOORD_{layer}' in a:
                uv = glb.accessor(gltf, blob, a[f'TEXCOORD_{layer}'])
                base = 'Alu' if re.search('alumin', name, re.I) else 'Black' if 'Material.001' in name else 'Carbon'
                tiles = np.floor(uv[tri][:, :, 0].mean(1)).astype(int).clip(0, 3)
                for t in np.unique(tiles):
                    sub = tri[tiles == t]
                    used, remap = np.unique(sub, return_inverse=True)
                    add(target, f'Liv{1001 + t}{base}', v[used] - origin, n[used], uv[used] - [t, 0], remap.reshape(-1, 3))
                    blender = lambda d: np.c_[d[:, 0], -d[:, 2], d[:, 1]]   # glTF to Blender axes
                    soups.setdefault(int(t), []).append((uv[sub], blender(v)[sub], blender(n)[sub]))
                    materials[f'Liv{1001 + t}{base}'] = {'kind': 'livery', 'tile': 1001 + int(t), 'base': base}
            else:
                if '[LIVERY]' in name:
                    no_uv.add(node_name)
                uv = glb.accessor(gltf, blob, a['TEXCOORD_0']) if 'TEXCOORD_0' in a else np.zeros((len(v), 2))
                add(target, describe(index), v - origin, n, uv, tri)

    for mesh, groups in meshes.items():
        prims = []
        for material, parts in groups.items():
            offsets = np.cumsum([0] + [len(p[0]) for p in parts[:-1]])
            prims.append((material, np.concatenate([p[0] for p in parts]), np.concatenate([p[1] for p in parts]),
                          np.concatenate([p[2] for p in parts]), np.concatenate([p[3].ravel() + o for p, o in zip(parts, offsets)])))
        glb.write(os.path.join(CAR, mesh + '.glb'), mesh, prims)
        print(mesh, sum(len(p[4]) // 3 for p in prims), 'triangles', len(prims), 'sections', flush=True)
    if no_uv:
        print('livery surfaces without LIVERY_UV (left as plain carbon):', sorted(no_uv))

    views = {k: np.asarray(Image.open(os.path.join(LIVERY, f'LIVERY_{k}.png')).convert('RGBA'), 'f4') / 255.0 for k in ('Left', 'Right', 'Top', 'Front', 'Rear')}
    sheets = {1001 + t: bake(t, soup, views) for t, soup in sorted(soups.items())}
    for spec in materials.values():
        if spec['kind'] == 'livery':
            color, metal, rough = LIVERY_BASE[spec['base']]
            spec.update(tex=sheets[spec['tile']], color=glb.srgb(color), metal=metal, rough=rough)
    json.dump(materials, open(os.path.join(CAR, 'materials.json'), 'w'), indent=1)


if __name__ == '__main__':
    main()
