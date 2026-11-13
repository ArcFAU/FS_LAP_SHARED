# Exports the full-detail car for Unreal to data\car\car_full.glb. Run in a BACKGROUND Blender only; it never saves livery.blend:
#   blender -b "E:\UE\livery\livery.blend" --python "E:\HOT LAP\high-end\tools\export_ue.py"
# Same steps as HoMo-LAP's blender\export_hotlap.py, without its decimation for the web.
import json
import os
import traceback

import bpy

OUT = os.path.join(r'E:\HOT LAP\high-end\data', 'car')
os.makedirs(OUT, exist_ok=True)
info = {}
try:
    bpy.context.window.scene = bpy.data.scenes['Scene']
    scene = bpy.context.scene
    # back plate sits in a hidden collection: show it in this background copy only
    plate = bpy.data.objects.get('SOLID003')
    bodywork = bpy.data.collections['01_Bodywork']
    if plate and plate.name not in bodywork.objects:
        bodywork.objects.link(plate)
    bpy.context.view_layer.update()
    visible = [o for o in scene.objects if o.type == 'MESH' and o.visible_get()]
    # The [LIVERY] node trees make the glTF exporter fail, and only their names matter: export plain stand-ins.
    for material in [m for m in bpy.data.materials if m.name.endswith('[LIVERY]')]:
        name = material.name
        material.name = name + '__orig'
        stand_in = bpy.data.materials.new(name)
        material.user_remap(stand_in)
    # With modifiers applied the exporter reads the mesh's data material, so copy object-linked ones into the mesh.
    for o in visible:
        if any(s.link == 'OBJECT' for s in o.material_slots):
            if o.data.users > 1:
                o.data = o.data.copy()
            for i, slot in enumerate(o.material_slots):
                if slot.link == 'OBJECT' and slot.material:
                    material = slot.material
                    o.data.materials[i] = material
                    slot.link = 'DATA'
    bpy.ops.export_scene.gltf(filepath=os.path.join(OUT, 'car_full.glb'), export_format='GLB', use_visible=True, export_apply=True,
                              export_yup=True, export_animations=False, export_cameras=False, export_lights=False, export_image_format='JPEG')
    # the channel that holds LIVERY_UV differs per object
    info['livery_uv_index'] = {o.name: [l.name for l in o.data.uv_layers].index('LIVERY_UV') for o in visible if 'LIVERY_UV' in [l.name for l in o.data.uv_layers]}
    info['ok'] = True
except Exception:
    info['error'] = traceback.format_exc()
json.dump(info, open(os.path.join(OUT, 'export_info.json'), 'w'), indent=1)
