# Blender (background, empty scene): place the exported CAD parts with the assembly positions, render check views, write a GLB.
#   blender -b --factory-startup --python assemble_car.py -- <tree.json> <stl folder> <out folder> [<keep.json> <name>]
# keep.json: a list of object names to keep (e.g. the owner's cleaned file plus parts to restore); name: output file name
# CAD frame kept: metres, x forward from the rear axle, y left, z up (Blender is z-up too).
import json
import math
import os
import re
import sys

import bpy
from mathutils import Matrix, Vector

args = sys.argv[sys.argv.index('--') + 1:]
tree_path, stl_dir, out_dir = args[:3]
keep = set(json.load(open(args[3]))) if len(args) > 3 else None
out_name = args[4] if len(args) > 4 else 'faumax_tau_cad'
tree = json.load(open(tree_path))
bpy.ops.wm.read_factory_settings(use_empty=True)
scene = bpy.context.scene

GROUPS = {'S': ('suspension', (0.55, 0.57, 0.6)), 'C': ('chassis', (0.12, 0.12, 0.13)), 'A': ('aero', (0.08, 0.08, 0.09)),
          'P': ('powertrain', (0.75, 0.45, 0.15)), 'E': ('electronics', (0.2, 0.45, 0.75))}
materials = {}
for key, (name, colour) in GROUPS.items():
    m = bpy.data.materials.new(name)
    m.diffuse_color = (*colour, 1)
    materials[key] = m

meshes, placed, missing, seen = {}, 0, set(), {}
for c in tree['components']:
    if c['suppressed']:
        continue
    stem = c['name'].lower()
    path = os.path.join(stl_dir, stem + '.stl')
    if not os.path.exists(path):
        missing.add(stem)
        continue
    if stem not in meshes:
        bpy.ops.wm.stl_import(filepath=path)   # mm, in the part's own frame
        obj = bpy.context.selected_objects[0]
        obj.data.transform(Matrix.Scale(0.001, 4))   # mm to m in the mesh itself: the mesh is shared by every instance
        meshes[stem] = obj.data
        bpy.data.objects.remove(obj)
    # NX gives the component's axes as rows in car coordinates: p_car = origin + px*X + py*Y + pz*Z
    X, Y, Z, o = Vector(c['xrow']), Vector(c['yrow']), Vector(c['zrow']), Vector(c['origin']) * 0.001
    # the name the full build gives this instance (base, base.001, ...), whatever is left out of this build
    n = seen.get(c['name'], 0)
    seen[c['name']] = n + 1
    # Blender reads a name ending in .<digits> (S_MP03_Conicalspacer_16x8x3.25) as a numbered copy and numbers from its stem
    name = c['name'] if n == 0 else '%s.%03d' % (re.sub(r'\.\d+$', '', c['name']), n)
    if keep is not None and name not in keep:
        continue
    obj = bpy.data.objects.new(name, meshes[stem])
    obj.matrix_world = Matrix(((X.x, Y.x, Z.x, o.x), (X.y, Y.y, Z.y, o.y), (X.z, Y.z, Z.z, o.z), (0, 0, 0, 1)))
    top = next((k for k in GROUPS if stem.startswith(k.lower())), 'S')
    if not obj.data.materials:
        obj.data.materials.append(materials[top])
    scene.collection.objects.link(obj)
    placed += 1

# hardpoints as small spheres, to check the parts sit on them
hp = json.load(open(os.path.join(os.path.dirname(tree_path), 'hardpoints.json')))['points']
dot = bpy.data.materials.new('hardpoint')
dot.diffuse_color = (1, 0.1, 0.1, 1)
for name, p in hp.items():
    for side in (1, -1):
        bpy.ops.mesh.primitive_uv_sphere_add(radius=0.012, location=(p['xyz'][0] / 1000, side * p['xyz'][1] / 1000, p['xyz'][2] / 1000))
        bpy.context.object.data.materials.append(dot)
        bpy.context.object.name = 'HP_' + name + ('_L' if side > 0 else '_R')

scene.render.engine = 'BLENDER_WORKBENCH'
scene.display.shading.color_type = 'MATERIAL'
scene.render.resolution_x, scene.render.resolution_y = 1600, 900
cam = bpy.data.objects.new('cam', bpy.data.cameras.new('cam'))
scene.collection.objects.link(cam)
scene.camera = cam
cam.data.type = 'ORTHO'
for view, loc, rot, scale in (('side', (0.75, -6, 0.45), (90, 0, 0), 3.6), ('top', (0.75, 0, 6), (0, 0, 90), 3.6),
                              ('front', (6, 0, 0.45), (90, 0, 90), 1.8), ('iso', (4.2, -3.6, 2.6), (62, 0, 50), 4.2), ('bottom', (0.75, 0, -6), (180, 0, 90), 3.6)):
    cam.location = loc
    cam.rotation_euler = [math.radians(a) for a in rot]
    cam.data.ortho_scale = scale
    scene.render.filepath = os.path.join(out_dir, out_name + '_' + view + '.png')
    bpy.ops.render.render(write_still=True)

bpy.ops.wm.save_as_mainfile(filepath=os.path.join(out_dir, out_name + '.blend'))   # with the hardpoint markers, for looking at
for o in [o for o in scene.objects if o.name.startswith('HP_') or o.type == 'CAMERA']:
    bpy.data.objects.remove(o)
bpy.ops.export_scene.gltf(filepath=os.path.join(out_dir, out_name + '.glb'), export_format='GLB', export_apply=False)
print('ASSEMBLED', placed, 'instances of', len(meshes), 'parts;', len(missing), 'components without an STL yet')
