"""Import generated FS Lap assets into the Unreal content browser and give them their real materials.

Run in a windowless full editor after the stage-1 generators (material editing needs it):
  UnrealEditor-Cmd.exe FSLapUE.uproject -ExecCmds="py import runpy; runpy.run_path('<this file>')" -unattended -nop4 -nosplash -nullrhi -nosound
"""
import json
import os

import unreal as ue


ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
DATA = os.path.join(ROOT, "data")
DEST = "/Game/FS/Generated"
MATERIALS = DEST + "/Materials"
ASSETS = ue.EditorAssetLibrary
TOOLS = ue.AssetToolsHelpers.get_asset_tools()
MAT = ue.MaterialEditingLibrary


def import_file(path, destination):
    task = ue.AssetImportTask()
    task.filename = path
    task.destination_path = destination
    task.automated = True
    task.replace_existing = True
    task.save = True
    TOOLS.import_asset_tasks([task])
    if not task.imported_object_paths:
        raise RuntimeError("Unreal imported no asset from " + path)
    return list(task.imported_object_paths)


def node(material, kind, x, y, name=None, value=None):
    expression = MAT.create_material_expression(material, kind, x, y)
    if name:
        expression.set_editor_property("parameter_name", name)
    if value is not None:
        expression.set_editor_property("default_value", value)
    return expression


def constant(material, value, y):
    expression = MAT.create_material_expression(material, ue.MaterialExpressionConstant, -300, y)
    expression.set_editor_property("r", value)
    return expression


def new_master(name, two_sided):
    material = TOOLS.create_asset(name, MATERIALS, ue.Material, ue.MaterialFactoryNew())
    material.set_editor_property("two_sided", two_sided)
    material.set_editor_property("used_with_nanite", True)
    material.set_editor_property("used_with_instanced_static_meshes", True)
    return material


def keep_or_build(name, build):
    """A master that already exists is kept. To change a graph, delete the asset first, with no other Unreal running."""
    path = MATERIALS + "/" + name
    return ASSETS.load_asset(path) if ASSETS.does_asset_exist(path) else finish(build())


def texture_node(material, default, y):
    expression = MAT.create_material_expression(material, ue.MaterialExpressionTextureSampleParameter2D, -700, y)
    expression.set_editor_property("parameter_name", "Tex")
    expression.set_editor_property("texture", default)
    return expression


def named_texture_node(material, parameter, default, y):
    expression = MAT.create_material_expression(material, ue.MaterialExpressionTextureSampleParameter2D, -700, y)
    expression.set_editor_property("parameter_name", parameter)
    expression.set_editor_property("texture", default)
    return expression


def finish(material):
    MAT.recompile_material(material)
    ASSETS.save_loaded_asset(material)
    return material


def scalars(material):
    metal = node(material, ue.MaterialExpressionScalarParameter, -700, 200, "Metal", 0.0)
    rough = node(material, ue.MaterialExpressionScalarParameter, -700, 300, "Rough", 0.6)
    return metal, rough


def color(material):
    return node(material, ue.MaterialExpressionVectorParameter, -700, -300, "Color", ue.LinearColor(0.5, 0.5, 0.5, 1.0))


def lerp(material, a, a_out, b, b_out, alpha, y):
    expression = MAT.create_material_expression(material, ue.MaterialExpressionLinearInterpolate, -200, y)
    MAT.connect_material_expressions(a, a_out, expression, "A")
    MAT.connect_material_expressions(b, b_out, expression, "B")
    MAT.connect_material_expressions(alpha, "A", expression, "Alpha")
    return expression


def build_facade(facade_texture):
    """Windows laid out by world position, so one texture fits every building size; each building gets its own tint."""
    material = new_master("M_Facade", False)
    position = MAT.create_material_expression(material, ue.MaterialExpressionWorldPosition, -1500, -200)
    normal = MAT.create_material_expression(material, ue.MaterialExpressionVertexNormalWS, -1500, 100)

    def channel(source, letter, y):
        mask = MAT.create_material_expression(material, ue.MaterialExpressionComponentMask, -1300, y)
        for name in "rgb":
            mask.set_editor_property(name, name == letter)
        MAT.connect_material_expressions(source, "", mask, "")
        return mask

    def absolute(source, y):
        node_abs = MAT.create_material_expression(material, ue.MaterialExpressionAbs, -1100, y)
        MAT.connect_material_expressions(source, "", node_abs, "")
        return node_abs

    def binary(kind, a, b, y, a_out="", b_out=""):
        expression = MAT.create_material_expression(material, kind, -900, y)
        MAT.connect_material_expressions(a, a_out, expression, "A")
        MAT.connect_material_expressions(b, b_out, expression, "B")
        return expression

    px, py, pz = channel(position, "r", -400), channel(position, "g", -300), channel(position, "b", -200)
    nx, ny, nz = absolute(channel(normal, "r", 0), 0), absolute(channel(normal, "g", 100), 100), absolute(channel(normal, "b", 200), 200)
    along = binary(ue.MaterialExpressionAdd, binary(ue.MaterialExpressionMultiply, px, ny, -400), binary(ue.MaterialExpressionMultiply, py, nx, -300), -350)
    bay_u, bay_v = MAT.create_material_expression(material, ue.MaterialExpressionConstant, -1000, -520), MAT.create_material_expression(material, ue.MaterialExpressionConstant, -1000, -600)
    bay_u.set_editor_property("r", 600.0)   # one bay of the texture is 6 m wide
    bay_v.set_editor_property("r", 700.0)   # and two 3.5 m floors tall
    uv = binary(ue.MaterialExpressionAppendVector, binary(ue.MaterialExpressionDivide, along, bay_u, -350), binary(ue.MaterialExpressionDivide, pz, bay_v, -200), -280)
    sample = texture_node(material, facade_texture, -250)
    MAT.connect_material_expressions(uv, "", sample, "UVs")

    random = MAT.create_material_expression(material, ue.MaterialExpressionPerInstanceRandom, -700, 100)
    warm, cool = MAT.create_material_expression(material, ue.MaterialExpressionConstant3Vector, -700, 200), MAT.create_material_expression(material, ue.MaterialExpressionConstant3Vector, -700, 300)
    warm.set_editor_property("constant", ue.LinearColor(0.85, 0.72, 0.58, 1.0))
    cool.set_editor_property("constant", ue.LinearColor(0.55, 0.62, 0.72, 1.0))
    tint = MAT.create_material_expression(material, ue.MaterialExpressionLinearInterpolate, -500, 150)
    MAT.connect_material_expressions(warm, "", tint, "A")
    MAT.connect_material_expressions(cool, "", tint, "B")
    MAT.connect_material_expressions(random, "", tint, "Alpha")
    wall = binary(ue.MaterialExpressionMultiply, sample, tint, -150, "RGB")
    roof_color = MAT.create_material_expression(material, ue.MaterialExpressionConstant3Vector, -500, 400)
    roof_color.set_editor_property("constant", ue.LinearColor(0.11, 0.11, 0.12, 1.0))
    final = MAT.create_material_expression(material, ue.MaterialExpressionLinearInterpolate, -300, 0)
    MAT.connect_material_expressions(wall, "", final, "A")
    MAT.connect_material_expressions(roof_color, "", final, "B")
    MAT.connect_material_expressions(nz, "", final, "Alpha")
    MAT.connect_material_property(final, "", ue.MaterialProperty.MP_BASE_COLOR)
    MAT.connect_material_property(constant(material, 0.6, 500), "", ue.MaterialProperty.MP_ROUGHNESS)
    return material


def build_flat():
    flat = new_master("M_Flat", True)
    metal, rough = scalars(flat)
    MAT.connect_material_property(color(flat), "", ue.MaterialProperty.MP_BASE_COLOR)
    MAT.connect_material_property(metal, "", ue.MaterialProperty.MP_METALLIC)
    MAT.connect_material_property(rough, "", ue.MaterialProperty.MP_ROUGHNESS)
    return flat


def build_textured(default_texture):
    textured = new_master("M_Tex", True)
    metal, rough = scalars(textured)
    sample = texture_node(textured, default_texture, 0)
    MAT.connect_material_property(sample, "RGB", ue.MaterialProperty.MP_BASE_COLOR)
    MAT.connect_material_property(sample, "A", ue.MaterialProperty.MP_OPACITY_MASK)
    MAT.connect_material_property(metal, "", ue.MaterialProperty.MP_METALLIC)
    MAT.connect_material_property(rough, "", ue.MaterialProperty.MP_ROUGHNESS)
    return textured


def build_livery(default_texture):
    # same blend as the web game's livery shader: paint over the base, paint is non-metal with roughness 0.3
    livery = new_master("M_Livery", True)
    metal, rough = scalars(livery)
    sample = texture_node(livery, default_texture, 0)
    MAT.connect_material_property(lerp(livery, color(livery), "", sample, "RGB", sample, -300), "", ue.MaterialProperty.MP_BASE_COLOR)
    MAT.connect_material_property(lerp(livery, metal, "", constant(livery, 0.0, 500), "", sample, 100), "", ue.MaterialProperty.MP_METALLIC)
    MAT.connect_material_property(lerp(livery, rough, "", constant(livery, 0.3, 600), "", sample, 300), "", ue.MaterialProperty.MP_ROUGHNESS)
    return livery


def build_ground(noise):
    ground = new_master("M_Ground", False)
    _, rough = scalars(ground)
    sample = texture_node(ground, noise, 0)
    shade = MAT.create_material_expression(ground, ue.MaterialExpressionMultiply, -350, -100)
    MAT.connect_material_expressions(color(ground), "", shade, "A")
    MAT.connect_material_expressions(sample, "R", shade, "B")
    doubled = MAT.create_material_expression(ground, ue.MaterialExpressionMultiply, -150, -100)
    MAT.connect_material_expressions(shade, "", doubled, "A")
    MAT.connect_material_expressions(constant(ground, 2.0, -100), "", doubled, "B")   # the noise averages 0.5
    MAT.connect_material_property(doubled, "", ue.MaterialProperty.MP_BASE_COLOR)
    MAT.connect_material_property(rough, "", ue.MaterialProperty.MP_ROUGHNESS)
    return ground


def build_pbr(base, normal, roughness):
    """Shared physically based material for the real road and hard-surface maps."""
    material = new_master("M_PBR", False)
    base_sample = named_texture_node(material, "Base", base, -200)
    normal_sample = named_texture_node(material, "Normal", normal, 0)
    rough_sample = named_texture_node(material, "RoughMap", roughness, 200)
    MAT.connect_material_property(base_sample, "RGB", ue.MaterialProperty.MP_BASE_COLOR)
    MAT.connect_material_property(normal_sample, "RGB", ue.MaterialProperty.MP_NORMAL)
    MAT.connect_material_property(rough_sample, "R", ue.MaterialProperty.MP_ROUGHNESS)
    return material


def translucent_master(name):
    material = new_master(name, True)
    material.set_editor_property("used_with_nanite", False)
    material.set_editor_property("blend_mode", ue.BlendMode.BLEND_TRANSLUCENT)
    material.set_editor_property("shading_model", ue.MaterialShadingModel.MSM_UNLIT)
    return material


def build_ghost():
    ghost = translucent_master("M_Ghost")
    MAT.connect_material_property(color(ghost), "", ue.MaterialProperty.MP_EMISSIVE_COLOR)
    MAT.connect_material_property(constant(ghost, 0.4, 200), "", ue.MaterialProperty.MP_OPACITY)
    return ghost


def build_smoke():
    """A soft pale puff: per-instance opacity, fading out towards the edge of the sphere."""
    smoke = translucent_master("M_Smoke")
    tone = MAT.create_material_expression(smoke, ue.MaterialExpressionConstant3Vector, -500, -200)
    tone.set_editor_property("constant", ue.LinearColor(0.7, 0.7, 0.72, 1.0))
    MAT.connect_material_property(tone, "", ue.MaterialProperty.MP_EMISSIVE_COLOR)
    life = MAT.create_material_expression(smoke, ue.MaterialExpressionPerInstanceCustomData, -700, 100)
    life.set_editor_property("data_index", 0)
    edge = MAT.create_material_expression(smoke, ue.MaterialExpressionFresnel, -700, 300)
    edge.set_editor_property("exponent", 1.2)   # a wide, soft falloff from the middle of the puff to its edge
    centre = MAT.create_material_expression(smoke, ue.MaterialExpressionOneMinus, -500, 300)
    MAT.connect_material_expressions(edge, "", centre, "")
    opacity = MAT.create_material_expression(smoke, ue.MaterialExpressionMultiply, -300, 200)
    MAT.connect_material_expressions(life, "", opacity, "A")
    MAT.connect_material_expressions(centre, "", opacity, "B")
    MAT.connect_material_property(opacity, "", ue.MaterialProperty.MP_OPACITY)
    return smoke


def build_masters(default_texture, noise, facade_texture):
    masters = {
        "flat": keep_or_build("M_Flat", build_flat),
        "tex": keep_or_build("M_Tex", lambda: build_textured(default_texture)),
        "livery": keep_or_build("M_Livery", lambda: build_livery(default_texture)),
        "ground": keep_or_build("M_Ground", lambda: build_ground(noise)),
        "ghost": keep_or_build("M_Ghost", build_ghost),
        "facade": keep_or_build("M_Facade", lambda: build_facade(facade_texture)),
    }
    keep_or_build("M_Smoke", build_smoke)   # used directly by the game, without instances
    return masters


def make_instance(name, spec, masters, textures):
    path = MATERIALS + "/MI_" + name
    instance = ASSETS.load_asset(path) if ASSETS.does_asset_exist(path) else TOOLS.create_asset("MI_" + name, MATERIALS, ue.MaterialInstanceConstant, ue.MaterialInstanceConstantFactoryNew())
    MAT.set_material_instance_parent(instance, masters[spec["kind"]])
    if spec["kind"] == "facade":   # everything is in the master
        MAT.update_material_instance(instance)
        ASSETS.save_loaded_asset(instance)
        return instance
    if spec["kind"] == "pbr":
        MAT.set_material_instance_texture_parameter_value(instance, "Base", textures[spec["base"]])
        MAT.set_material_instance_texture_parameter_value(instance, "Normal", textures[spec["normal"]])
        MAT.set_material_instance_texture_parameter_value(instance, "RoughMap", textures[spec["roughmap"]])
    elif spec["kind"] != "tex":
        MAT.set_material_instance_vector_parameter_value(instance, "Color", ue.LinearColor(*spec["color"], 1.0))
    if spec["kind"] not in ("ghost", "pbr"):
        MAT.set_material_instance_scalar_parameter_value(instance, "Rough", spec["rough"])
    if spec["kind"] not in ("ground", "ghost", "pbr"):
        MAT.set_material_instance_scalar_parameter_value(instance, "Metal", spec["metal"])
    if spec.get("tex"):
        MAT.set_material_instance_texture_parameter_value(instance, "Tex", textures[spec["tex"]])
    if spec.get("masked"):
        overrides = instance.get_editor_property("base_property_overrides")
        overrides.set_editor_property("override_blend_mode", True)
        overrides.set_editor_property("blend_mode", ue.BlendMode.BLEND_MASKED)
        instance.set_editor_property("base_property_overrides", overrides)
    MAT.update_material_instance(instance)
    ASSETS.save_loaded_asset(instance)
    return instance


def assign(mesh_path, instances):
    mesh = ASSETS.load_asset(mesh_path)
    for index, slot in enumerate(mesh.get_editor_property("static_materials")):
        name = str(slot.get_editor_property("material_slot_name"))
        if name not in instances:
            raise RuntimeError(f"no material for slot {name} of {mesh_path}")
        mesh.set_material(index, instances[name])
    ASSETS.save_loaded_asset(mesh)


def city_ground_only():
    """FSLAP_IMPORT=city: the new ground mesh only, using the materials that exist, so nothing another Unreal has open is re-saved."""
    names = ("CityFar", "CityAsphalt", "CitySidewalk", "CityLine", "Line", "Grass")
    instances = {name: ASSETS.load_asset(MATERIALS + "/MI_" + name) for name in names}
    for path in import_file(os.path.join(DATA, "city", "SM_CityBig.glb"), DEST + "/City"):
        if isinstance(ASSETS.load_asset(path), ue.StaticMesh):
            assign(path, instances)
            ASSETS.delete_directory(path.rsplit("/", 2)[0] + "/Materials")
    print("FSLAP_IMPORT_OK city")


def environment_only():
    """FSLAP_IMPORT=environment: the CC0 PBR asphalt and concrete of the city streets (tools\\fetch_environment_assets.py)."""
    textures = {}
    # The CC0 source and the exact selected maps are captured by the fetch tool's manifest.
    environment = os.path.join(DATA, "environment")
    manifest = json.load(open(os.path.join(environment, "manifest.json")))
    for material in manifest["materials"].values():
        for file in material["maps"].values():
            if file not in textures:
                imported = import_file(os.path.join(environment, file), DEST + "/Environment")
                textures[file] = ASSETS.load_asset(imported[0])
    masters = {
        "pbr": keep_or_build("M_PBR", lambda: build_pbr(textures["asphalt_base.jpg"], textures["asphalt_normal.jpg"], textures["asphalt_rough.jpg"])),
    }
    pbr = lambda material: {"kind": "pbr", "base": f"{material}_base.jpg", "normal": f"{material}_normal.jpg", "roughmap": f"{material}_rough.jpg"}
    # updating the instances in place gives the city ground mesh the new look without reimporting it
    for name, material in (("CityAsphalt", "asphalt"), ("CitySidewalk", "concrete")):
        make_instance(name, pbr(material), masters, textures)
    print("FSLAP_IMPORT_OK environment")


def cad_only():
    """FSLAP_IMPORT=cad: the car's mechanical parts from the team CAD (tools\\cad_to_ue.py) and their flat materials."""
    specs = json.load(open(os.path.join(DATA, "car", "cad_materials.json")))
    instances = {name: make_instance(name, spec, {"flat": ASSETS.load_asset(MATERIALS + "/M_Flat")}, {}) for name, spec in specs.items()}
    instances["Carbon"] = ASSETS.load_asset(MATERIALS + "/MI_Carbon")   # the body's own carbon
    count = 0
    for corner in ("Body", "Hub_FL", "Hub_FR", "Hub_RL", "Hub_RR", "Spin_FL", "Spin_FR", "Spin_RL", "Spin_RR", "SteeringWheel"):
        for path in import_file(os.path.join(DATA, "car", "SM_Cad_" + corner + ".glb"), DEST + "/Car"):
            if isinstance(ASSETS.load_asset(path), ue.StaticMesh):
                assign(path, instances)
                ASSETS.delete_directory(path.rsplit("/", 2)[0] + "/Materials")
                count += 1
    print("FSLAP_IMPORT_OK cad " + str(count))


def event_grounds():
    """FSG's ground.glb and every ground_<id>.glb of the other event tracks (tools\\track_to_ue.py)"""
    return ["ground"] + sorted(f[:-4] for f in os.listdir(os.path.join(DATA, "track")) if f.startswith("ground_") and f.endswith(".glb"))


def tracks_only():
    """FSLAP_IMPORT=tracks: the event tracks' grounds with the track materials that exist."""
    instances = {name: ASSETS.load_asset(MATERIALS + "/MI_" + name) for name in ("Grass", "Apron", "Road", "Line")}
    for name in event_grounds():
        for path in import_file(os.path.join(DATA, "track", name + ".glb"), DEST + "/Track"):
            if isinstance(ASSETS.load_asset(path), ue.StaticMesh):
                assign(path, instances)
                ASSETS.delete_directory(path.rsplit("/", 2)[0] + "/Materials")
    print("FSLAP_IMPORT_OK tracks " + " ".join(event_grounds()))


def main():
    if os.environ.get("FSLAP_IMPORT") == "tracks":
        tracks_only()
        return
    if os.environ.get("FSLAP_IMPORT") == "cad":
        cad_only()
        return
    if os.environ.get("FSLAP_IMPORT") == "city":
        city_ground_only()
        return
    if os.environ.get("FSLAP_IMPORT") == "environment":
        environment_only()
        return
    if not os.path.isdir(os.path.join(DATA, "car")):
        raise RuntimeError("missing generated data\\car; run the export and bake tools first")
    if not os.path.isdir(os.path.join(DATA, "track")):
        raise RuntimeError("missing generated data\\track; run tools\\track_to_ue.py first")

    ASSETS.make_directory(DEST)
    ASSETS.make_directory(MATERIALS)
    meshes = []
    for name in ("SM_Body", "SM_Wheel_FL", "SM_Wheel_FR", "SM_Wheel_RL", "SM_Wheel_RR", "SM_SteeringWheel"):
        meshes += import_file(os.path.join(DATA, "car", name + ".glb"), DEST + "/Car")
    for name in event_grounds() + ["paddock", "skid", "cone_b", "cone_y", "cone_o"]:
        meshes += import_file(os.path.join(DATA, "track", name + ".glb"), DEST + "/Track")
    for name in ("SM_CityBig", "SM_CityBox", "SM_CityBarrier", "SM_CityPole"):
        meshes += import_file(os.path.join(DATA, "city", name + ".glb"), DEST + "/City")
    meshes = [path for path in meshes if isinstance(ASSETS.load_asset(path), ue.StaticMesh)]   # the import also lists the materials it made

    specs = json.load(open(os.path.join(DATA, "car", "materials.json")))
    specs.update(json.load(open(os.path.join(DATA, "track", "materials.json"))))
    specs.update(json.load(open(os.path.join(DATA, "city", "materials.json"))))
    specs.update(GhostBest={"kind": "ghost", "color": [0.2, 0.3, 0.42]}, GhostLts={"kind": "ghost", "color": [0.55, 0.32, 0.03]})
    textures = {}
    for spec in specs.values():
        file = spec.get("tex")
        if file and file not in textures:
            folder = "city" if os.path.exists(os.path.join(DATA, "city", file)) else "car"
            imported = import_file(os.path.join(DATA, folder, file), DEST + ("/City" if folder == "city" else "/Car/Textures"))
            textures[file] = ASSETS.load_asset(imported[0])
    noise = ASSETS.load_asset(import_file(os.path.join(DATA, "track", "noise.png"), DEST + "/Track")[0])
    noise.set_editor_property("srgb", False)
    ASSETS.save_loaded_asset(noise)

    masters = build_masters(next(iter(textures.values())), noise, textures["T_Facade.png"])
    instances = {name: make_instance(name, spec, masters, textures) for name, spec in specs.items()}
    for path in meshes:
        assign(path, instances)
        # shortcut: Interchange always makes empty materials next to a mesh, so they are removed here; an override pipeline asset would avoid them.
        ASSETS.delete_directory(path.rsplit("/", 2)[0] + "/Materials")

    with open(os.path.join(DATA, "import_manifest.json"), "w", encoding="utf-8") as output:
        json.dump({"destination": DEST, "meshes": meshes, "textures": sorted(textures), "source": DATA}, output, indent=2)
    print("FSLAP_IMPORT_OK " + str(len(meshes)))


try:
    main()
except Exception as error:
    ue.log_error("FSLAP_IMPORT_FAILED " + repr(error))
    raise
finally:
    ue.SystemLibrary.quit_editor()
