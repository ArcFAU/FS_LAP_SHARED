# Blender (background): every object of the open .blend with its mesh, triangle count and world bounds, to JSON.
#   blender -b <file.blend> --python list_objects.py -- <out.json>
import json
import sys

import bpy
from mathutils import Vector

out = []
for o in bpy.data.objects:
    row = {'name': o.name, 'type': o.type, 'mesh': o.data.name if o.data else '', 'hidden': o.hide_get() or o.hide_viewport or o.hide_render}
    if o.type == 'MESH':
        pts = [o.matrix_world @ Vector(c) for c in o.bound_box]
        row['lo'] = [round(min(p[i] for p in pts), 3) for i in range(3)]
        row['hi'] = [round(max(p[i] for p in pts), 3) for i in range(3)]
        row['tris'] = sum(len(p.vertices) - 2 for p in o.data.polygons)
    out.append(row)
json.dump(out, open(sys.argv[sys.argv.index('--') + 1], 'w'), indent=0)
print('LISTED', len(out))
