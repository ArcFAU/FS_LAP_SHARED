# The car's mechanical parts from the team CAD for FS Lap: suspension, uprights, brakes, hubs, motors, steering rack.
# Run in a BACKGROUND Blender on the owner's cleaned CAD car; it never saves that file:
#   blender -b D:\FSLap_CAD\out\preview\faumax_tau_cad_clean_fixed.blend --python tools\cad_to_ue.py -- [--dry]
# Source: D:\FSLap_CAD (copied and exported from the team's NX model, see its README). Output in data\car (git-ignored):
#   SM_Cad_Body.glb             parts fixed to the body (origin midway between the axles, like SM_Body)
#   SM_Cad_Hub_FL/FR/RL/RR.glb  parts that steer with the wheel but don't spin (origin at the wheel centre)
#   SM_Cad_Spin_FL/FR/RL/RR.glb parts that spin with the wheel: disc, hub (origin at the wheel centre)
#   SM_Cad_SteeringWheel.glb    the team's steering wheel with its quick release and buttons, origin on its axis and the axis laid along x (the game tilts and turns it);
#                               the position and column angle the game needs are printed and written to steering_wheel.json
#   cad_materials.json          their materials, for tools\import_ue.py
# Parts the game model (livery.blend) already has are left out, and so are hidden internals.
import json
import math
import os
import re
import struct
import sys

import bpy
import numpy as np
from mathutils import Matrix

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
OUT = os.path.join(ROOT, 'data', 'car')
TREE = r'D:\FSLap_CAD\out\tree.json'
STL = r'D:\FSLap_CAD\out\stl7'
GAME = os.path.join(os.path.dirname(ROOT), 'blender', 'faumax_tau.glb')   # the livery model the game is built from
DRY = '--dry' in sys.argv
BUDGET = 250_000   # triangles for everything added; the body itself is about 660k
WHEEL_CENTRE = {'FL': (1530, 620), 'FR': (1530, -620), 'RL': (0, 600), 'RR': (0, -600)}   # CAD mm (x, y); centre height 200 mm
CORNER = {'S_FL07_WHEELASSEMBLY': 'FL', 'S_FR07_WHEEL_ASSEMBLY': 'FR', 'S_RL07_WHEEL_ASSEMBLY': 'RL', 'S_RR07_WHEEL_ASSEMBLY': 'RR'}
# hidden or not wanted in the game: bearings, fasteners, o-rings, gearbox internals, hose fittings, connectors,
# the pedal box (inside the nose), EBS pneumatics
SKIP = re.compile(r'^(\d|b7\d|f_699|axial|needle|o-ring|d9\d\d|d7991|m80-|splint|norelem|speedi|wa 67|40x56|askubal|hfr[cl]|c-2387380|11096515|motor_stecker|rohrkabel|gtc06'
                  r'|p_mo02_(planet|plantet|nadel|thrust|bearing)|p_mo02_ringgear|s_p[bg]\d|e_sp\d|s_ep'
                  r'|c_mo01_tau_mono|s_fl07_topo_420g)', re.I)   # topo: a second, optimisation-study upright overlapping S_FL07_Upright (only front left has it)
SPIN = re.compile(r'brakedisc|wheelhub|_wh_|pretension|conicalspacer', re.I)
WHEEL = re.compile(r'^s_sw|tau lenkrad', re.I)   # steering wheel, quick release, buttons (the display E_LV12_FIS is a fixed dash display)


def game_parts():
    """normalised part names of the game's livery model"""
    b = open(GAME, 'rb').read()
    n = struct.unpack('<I', b[12:16])[0]
    norm = lambda s: re.sub(r'[^a-z0-9]', '', re.sub(r'(\.\d{3}|\d{3})$', '', s.lower()))
    return {norm(nd.get('name', '')) for nd in json.loads(b[20:20 + n])['nodes']}, norm


def instance_paths():
    """Blender object name -> CAD assembly path, with the same naming as D:\\FSLap_CAD\\tools\\assemble_car.py"""
    paths, seen, stack = {}, {}, []
    for c in json.load(open(TREE))['components']:
        stack = stack[:c['level'] - 1] + [c['name']]
        if c['suppressed'] or not os.path.exists(os.path.join(STL, c['name'].lower() + '.stl')):
            continue
        n = seen.get(c['name'], 0)
        seen[c['name']] = n + 1
        name = c['name'] if n == 0 else '%s.%03d' % (re.sub(r'\.\d+$', '', c['name']), n)
        paths[name] = stack[:-1]
    return paths


def material(base):
    b = base.lower()
    if 'lenkrad' in b:
        return 'Carbon'
    if re.match(r's_sw09|e_lv12', b):
        return 'CadDark'   # buttons, plugs, display
    if 'brakedisc' in b:
        return 'CadDisc'
    if re.search(r'tube|_rod|pushrod|tierod|toe', b):
        return 'Carbon'   # the game's own carbon
    if re.search(r'motor|cooling_jacket|caliper|brake_ap|^e_|cable|hv_', b):
        return 'CadDark'
    return 'CadAlu'


def self_check():
    assert SKIP.search('C_MO01_Tau_Mono_v11_5 experiment')   # in the game model as MONO (same shape within 0.5 mm)
    assert SKIP.search('61902') and SKIP.search('P_MO02_Planet_1') and SKIP.search('s_pb02_brake_pedal_v2_4') and not SKIP.search('S_FL07_Upright')
    assert SPIN.search('S_MP03_Brakedisc') and not SPIN.search('S_MP05_Brake_AP_SMALL')
    assert material('S_FL02_UP_REAR_TUBE') == 'Carbon' and material('S_MP03_Brakedisc') == 'CadDisc'
    assert WHEEL.search('tau Lenkrad v11_step') and not WHEEL.search('E_LV12_FIS') and not WHEEL.search('S_SC03_INSERT_SPLINTED_JOINT')


self_check()
game, norm = game_parts()
paths = instance_paths()
groups, skipped, unplaced = {}, [], []
for o in bpy.data.objects:
    if o.type != 'MESH' or o.name.startswith('HP_'):
        continue
    base = re.sub(r'\.\d{3}$', '', o.name)
    if norm(base) in game:
        continue
    if SKIP.search(base):
        skipped.append(base)
        continue
    if o.name == 'E_LV12_FIS.001':
        continue   # the dash display is in the model twice, in the low-voltage assembly and the wiring harness, 5 mm apart
    if WHEEL.search(base):
        groups.setdefault('SteeringWheel', []).append(o)
        continue
    path = paths.get(o.name)
    if path is None:
        unplaced.append(o.name)
        continue
    corner = next((CORNER[p] for p in path if p in CORNER), None)
    key = 'Body' if corner is None else ('Spin_' if SPIN.search(base) else 'Hub_') + corner
    groups.setdefault(key, []).append(o)

tris = lambda o: sum(len(p.vertices) - 2 for p in o.data.polygons)
total = sum(tris(o) for objs in groups.values() for o in objs)
# one ratio for all, a floor so small parts keep their shape, a cap so the topology-optimised uprights don't eat the budget
ratio = BUDGET / max(total, 1)
target = lambda t: min(t, max(150, int(t * ratio * 1.6)), 6000)
wheel_target = lambda t: min(t, 12000)   # the steering wheel is right in front of the onboard camera
print('CAD_GROUPS', {k: (len(v), sum(tris(o) for o in v), sum(target(tris(o)) for o in v)) for k, v in sorted(groups.items())})
print('CAD_TOTAL before', total, 'after', sum(target(tris(o)) for v in groups.values() for o in v), '| skipped', len(skipped), sorted(set(skipped))[:30], '| unplaced', unplaced[:10])
if DRY:
    sys.exit(0)

materials = {}
for name, colour, metal, rough in (('CadAlu', (0.62, 0.64, 0.67), 0.9, 0.35), ('CadDisc', (0.42, 0.43, 0.45), 1.0, 0.45), ('CadDark', (0.05, 0.05, 0.055), 0.3, 0.5)):
    m = bpy.data.materials.get(name) or bpy.data.materials.new(name)
    m.diffuse_color = (*colour, 1)
    materials[name] = {'kind': 'flat', 'color': list(colour), 'metal': metal, 'rough': rough}
if not bpy.data.materials.get('Carbon'):
    bpy.data.materials.new('Carbon')   # the game's own carbon, matched by name on import
SLOTS = ['CadAlu', 'CadDisc', 'CadDark', 'Carbon']   # the same slot order in every part, so the joined mesh keeps each part's material

for key, objs in sorted(groups.items()):
    parts = []
    for o in objs:
        base = re.sub(r'\.\d{3}$', '', o.name)
        t = tris(o)
        goal = wheel_target(t) if key == 'SteeringWheel' else target(t)
        if goal < t:
            mod = o.modifiers.new('cad_dec', 'DECIMATE')
            mod.ratio = goal / t
            mod.use_collapse_triangulate = True
        mesh = bpy.data.meshes.new_from_object(o.evaluated_get(bpy.context.evaluated_depsgraph_get()))
        mesh.transform(o.matrix_world)
        mesh.materials.clear()
        for slot in SLOTS:
            mesh.materials.append(bpy.data.materials[slot])
        index = SLOTS.index(material(base))
        for p in mesh.polygons:
            p.material_index = index
        parts.append(mesh)
    # origin: the body's is midway between the axles, a corner's is its wheel centre (CAD metres, z up)
    turn = Matrix.Identity(4)
    if key == 'Body':
        ox, oy, oz = 0.765, 0.0, 0.0
    elif key == 'SteeringWheel':
        # the wheel's axis is its rim's thinnest direction; the centre is the middle of the rim
        rim = next(o for o in objs if WHEEL.search(o.name) and 'lenkrad' in o.name.lower())
        v = np.array([rim.matrix_world @ x.co for x in rim.data.vertices])
        centre = (v.min(0) + v.max(0)) / 2
        axis = np.linalg.eigh(np.cov((v - v.mean(0)).T))[1][:, 0]
        axis = axis if axis[0] > 0 else -axis   # pointing forward, down the column
        tilt = float(math.atan2(axis[2], axis[0]))   # negative: the column runs forward and down
        ox, oy, oz = (float(c) for c in centre)
        turn = Matrix.Rotation(tilt, 4, 'Y')    # lays the axis along x; the game tilts it back by the same angle
        wheel = {'position_cm_from_mid_axle': [round((ox - 0.765) * 100, 1), round(-oy * 100, 1), round(oz * 100, 1)], 'tilt_deg': round(math.degrees(tilt), 1)}
        json.dump(wheel, open(os.path.join(OUT, 'steering_wheel.json'), 'w'), indent=1)
        print('CAD_STEERING_WHEEL', wheel)
    else:
        cx, cy = WHEEL_CENTRE[key[-2:]]
        ox, oy, oz = cx / 1000, cy / 1000, 0.2
    joined = bpy.data.meshes.new('SM_Cad_' + key)
    import bmesh
    bm = bmesh.new()
    for mesh in parts:
        bm.from_mesh(mesh)
    bm.to_mesh(joined)
    bm.free()
    for slot in SLOTS:
        joined.materials.append(bpy.data.materials[slot])
    obj = bpy.data.objects.new('SM_Cad_' + key, joined)
    joined.transform(turn @ Matrix.Translation((-ox, -oy, -oz)))
    scene = bpy.context.scene
    scene.collection.objects.link(obj)
    for other in scene.objects:
        other.select_set(other == obj)
    bpy.context.view_layer.objects.active = obj
    # Blender z-up to glTF y-up: glTF (x, z, -y), i.e. x forward, y up, z to the car's right, as the game's car
    bpy.ops.export_scene.gltf(filepath=os.path.join(OUT, 'SM_Cad_' + key + '.glb'), export_format='GLB', use_selection=True, export_apply=False,
                              export_yup=True, export_animations=False, export_cameras=False, export_lights=False)
    print('CAD_WROTE', 'SM_Cad_' + key, sum(len(p.vertices) - 2 for p in joined.polygons), 'triangles')
json.dump(materials, open(os.path.join(OUT, 'cad_materials.json'), 'w'), indent=1)
print('CAD_DONE')
