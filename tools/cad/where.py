# Blender (background): world bounds of objects whose name contains the given text
import sys
import bpy
from mathutils import Vector
key = sys.argv[sys.argv.index('--') + 1].lower()
for o in bpy.data.objects:
    if key in o.name.lower() and o.type == 'MESH':
        pts = [o.matrix_world @ Vector(c) for c in o.bound_box]
        print('WHERE', o.name, len(o.data.polygons), 'tris', [round(min(p[i] for p in pts), 3) for i in range(3)], [round(max(p[i] for p in pts), 3) for i in range(3)])
