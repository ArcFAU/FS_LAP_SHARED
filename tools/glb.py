# Minimal glTF binary reader and writer for the FS Lap tools (no Blender or Unreal needed).
import json
import struct

import numpy as np

COMPONENTS = {5126: 'f4', 5125: 'u4', 5123: 'u2', 5121: 'u1'}
SIZES = {'SCALAR': 1, 'VEC2': 2, 'VEC3': 3, 'VEC4': 4}


def srgb(value):
    """0xRRGGBB to linear RGB."""
    c = np.array([(value >> 16) & 255, (value >> 8) & 255, value & 255]) / 255.0
    return np.where(c <= 0.04045, c / 12.92, ((c + 0.055) / 1.055) ** 2.4).round(5).tolist()


def read(path):
    data = open(path, 'rb').read()
    size = struct.unpack('<I', data[12:16])[0]
    return json.loads(data[20:20 + size]), data[20 + size + 8:]


def accessor(gltf, blob, index):
    a = gltf['accessors'][index]
    view = gltf['bufferViews'][a['bufferView']]
    n = SIZES[a['type']]
    values = np.frombuffer(blob, COMPONENTS[a['componentType']], a['count'] * n, view.get('byteOffset', 0) + a.get('byteOffset', 0))
    return values if n == 1 else values.reshape(-1, n)


def write(path, name, primitives):
    """primitives: list of (material name, positions Nx3, normals Nx3, uvs Nx2, indices 1-D), metres, glTF axes."""
    blob, views, accessors, prims, materials = bytearray(), [], [], [], []

    def add(values, component, kind, target):
        while len(blob) % 4:
            blob.append(0)
        raw = values.tobytes()
        views.append({'buffer': 0, 'byteOffset': len(blob), 'byteLength': len(raw), 'target': target})
        blob.extend(raw)
        entry = {'bufferView': len(views) - 1, 'componentType': component, 'count': len(values), 'type': kind}
        if kind == 'VEC3':
            entry['min'] = values.min(0).tolist()
            entry['max'] = values.max(0).tolist()
        accessors.append(entry)
        return len(accessors) - 1

    for material, p, n, uv, idx in primitives:
        if material not in materials:
            materials.append(material)
        prims.append({
            'attributes': {
                'POSITION': add(np.asarray(p, 'f4'), 5126, 'VEC3', 34962),
                'NORMAL': add(np.asarray(n, 'f4'), 5126, 'VEC3', 34962),
                'TEXCOORD_0': add(np.asarray(uv, 'f4'), 5126, 'VEC2', 34962),
            },
            'indices': add(np.asarray(idx, 'u4'), 5125, 'SCALAR', 34963),
            'material': materials.index(material),
        })
    gltf = {
        'asset': {'version': '2.0', 'generator': 'FS Lap tools'}, 'scene': 0, 'scenes': [{'nodes': [0]}],
        'nodes': [{'mesh': 0, 'name': name}], 'meshes': [{'name': name, 'primitives': prims}],
        'materials': [{'name': m} for m in materials],
        'buffers': [{'byteLength': len(blob)}], 'bufferViews': views, 'accessors': accessors,
    }
    head = json.dumps(gltf, separators=(',', ':')).encode()
    head += b' ' * (-len(head) % 4)
    blob.extend(b'\0' * (-len(blob) % 4))
    with open(path, 'wb') as out:
        out.write(struct.pack('<III', 0x46546C67, 2, 12 + 8 + len(head) + 8 + len(blob)))
        out.write(struct.pack('<II', len(head), 0x4E4F534A) + head)
        out.write(struct.pack('<II', len(blob), 0x004E4942) + bytes(blob))
