# Blender (background): list objects of the assembled car that lie outside the car's envelope.
#   blender -b <faumax_tau_cad.blend> --python find_strays.py
import json

import bpy
from mathutils import Vector

# the car: rear wing behind x = -0.6 m, front wing to about 2.3 m, half width 0.75 m, ground at z = 0
X0, X1, Y, Z0, Z1 = -0.75, 2.45, 0.80, -0.02, 1.35
out = []
for o in bpy.data.objects:
    if o.type != 'MESH' or o.name.startswith('HP_'):
        continue
    pts = [o.matrix_world @ Vector(c) for c in o.bound_box]
    lo = Vector([min(p[i] for p in pts) for i in range(3)])
    hi = Vector([max(p[i] for p in pts) for i in range(3)])
    if lo.x < X0 or hi.x > X1 or lo.y < -Y or hi.y > Y or lo.z < Z0 or hi.z > Z1:
        out.append({'name': o.name, 'mesh': o.data.name, 'lo': [round(v, 3) for v in lo], 'hi': [round(v, 3) for v in hi]})
json.dump(out, open(r'D:\FSLap_CAD\out\strays.json', 'w'), indent=1)
print('STRAYS', len(out))
for s in out:
    print('STRAY', s['name'], s['lo'], s['hi'])
