# Builds /Game/Test/GpuTest, the scene for the stage 0 GPU benchmark.
# Run by tools\gpu_test\run.ps1 in a windowless full editor (-ExecCmds="py ..."): placing actors
# needs the editor's actor factories, which the lighter -run=pythonscript mode doesn't set up.
# A room with one red and one green wall (so bounced light is visible), 400 Nanite spheres,
# sun, sky and atmosphere. Everything it creates lives under /Game/Test.
import unreal as ue

FOLDER = '/Game/Test'
MAP = FOLDER + '/GpuTest'
GRID = 20

assets = ue.EditorAssetLibrary
materials = ue.MaterialEditingLibrary
tools = ue.AssetToolsHelpers.get_asset_tools()
level = ue.get_editor_subsystem(ue.LevelEditorSubsystem)
actors = ue.get_editor_subsystem(ue.EditorActorSubsystem)


def nanite_copy(source, name):
    path = f'{FOLDER}/{name}'
    mesh = assets.load_asset(path) if assets.does_asset_exist(path) else assets.duplicate_asset(source, path)
    settings = mesh.get_editor_property('nanite_settings')
    settings.enabled = True
    mesh.set_editor_property('nanite_settings', settings)   # also rebuilds the mesh
    assets.save_loaded_asset(mesh)
    return mesh


def base_material():
    path = f'{FOLDER}/M_Test'
    if assets.does_asset_exist(path):
        material = assets.load_asset(path)
    else:
        material = tools.create_asset('M_Test', FOLDER, ue.Material, ue.MaterialFactoryNew())
        colour = materials.create_material_expression(material, ue.MaterialExpressionVectorParameter, -300, 0)
        colour.set_editor_property('parameter_name', 'Color')
        colour.set_editor_property('default_value', ue.LinearColor(0.5, 0.5, 0.5, 1.0))
        materials.connect_material_property(colour, '', ue.MaterialProperty.MP_BASE_COLOR)
    material.set_editor_property('used_with_nanite', True)   # without it the game falls back to the default material
    materials.recompile_material(material)
    assets.save_loaded_asset(material)
    return material


def tint(parent, name, r, g, b):
    path = f'{FOLDER}/{name}'
    if assets.does_asset_exist(path):
        return assets.load_asset(path)
    instance = tools.create_asset(name, FOLDER, ue.MaterialInstanceConstant, ue.MaterialInstanceConstantFactoryNew())
    materials.set_material_instance_parent(instance, parent)
    materials.set_material_instance_vector_parameter_value(instance, 'Color', ue.LinearColor(r, g, b, 1.0))
    materials.update_material_instance(instance)
    assets.save_loaded_asset(instance)
    return instance


def box(mesh, material, location, scale):
    actor = actors.spawn_actor_from_object(mesh, ue.Vector(*location), ue.Rotator())
    actor.set_actor_scale3d(ue.Vector(*scale))
    actor.static_mesh_component.set_material(0, material)
    return actor


def main():
    if level is None or actors is None:
        raise RuntimeError('editor subsystems are not available in this mode')
    if not assets.does_directory_exist(FOLDER):
        assets.make_directory(FOLDER)
    cube = nanite_copy('/Engine/BasicShapes/Cube', 'SM_NaniteCube')
    sphere = nanite_copy('/Engine/BasicShapes/Sphere', 'SM_NaniteSphere')
    parent = base_material()
    grey = tint(parent, 'MI_Grey', 0.45, 0.45, 0.45)
    red = tint(parent, 'MI_Red', 0.8, 0.05, 0.05)
    green = tint(parent, 'MI_Green', 0.05, 0.7, 0.1)
    white = tint(parent, 'MI_White', 0.8, 0.8, 0.8)

    if assets.does_asset_exist(MAP):
        assets.delete_asset(MAP)
    if not level.new_level(MAP, False):
        raise RuntimeError('could not create ' + MAP)

    box(cube, grey, (0, 0, -10), (400, 400, 0.2))        # ground, 400 m square
    box(cube, red, (900, -1000, 300), (18, 0.2, 6))      # left wall
    box(cube, green, (900, 1000, 300), (18, 0.2, 6))     # right wall
    box(cube, white, (1800, 0, 300), (0.2, 20, 6))       # back wall
    box(cube, white, (1200, 0, 600), (12, 20, 0.2))      # ceiling over the back of the room

    for i in range(GRID):
        for j in range(GRID):
            x = 200 + i * 1500.0 / (GRID - 1)
            y = -850 + j * 1700.0 / (GRID - 1)
            box(sphere, white if (i + j) % 2 else grey, (x, y, 40), (0.8, 0.8, 0.8))

    sun = actors.spawn_actor_from_class(ue.DirectionalLight, ue.Vector(0, 0, 2000), ue.Rotator(0, -40, 35))
    sun_light = sun.get_component_by_class(ue.DirectionalLightComponent)
    sun_light.set_editor_property('mobility', ue.ComponentMobility.MOVABLE)
    sun_light.set_editor_property('atmosphere_sun_light', True)

    sky = actors.spawn_actor_from_class(ue.SkyLight, ue.Vector(0, 0, 1000), ue.Rotator())
    sky_light = sky.get_component_by_class(ue.SkyLightComponent)
    sky_light.set_editor_property('mobility', ue.ComponentMobility.MOVABLE)
    sky_light.set_editor_property('real_time_capture', True)

    actors.spawn_actor_from_class(ue.SkyAtmosphere, ue.Vector(0, 0, 0), ue.Rotator())
    actors.spawn_actor_from_class(ue.PlayerStart, ue.Vector(-900, 0, 260), ue.Rotator(0, -4, 0))

    level.save_current_level()
    assets.save_directory(FOLDER)
    print('FSLAP_MAP_OK ' + MAP)


try:
    main()
except Exception as error:
    ue.log_error('FSLAP_MAP_FAILED ' + repr(error))
    raise
finally:
    ue.SystemLibrary.quit_editor()   # the script runs inside a full editor start; end it
