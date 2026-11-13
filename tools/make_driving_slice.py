"""Create the first playable Unreal driving slice from the imported ground mesh."""
import unreal as ue


FOLDER = "/Game/FS"
MAP = FOLDER + "/DrivingSlice_v1"
ASSETS = ue.EditorAssetLibrary
LEVEL = ue.get_editor_subsystem(ue.LevelEditorSubsystem)
ACTORS = ue.get_editor_subsystem(ue.EditorActorSubsystem)


def main():
    if ASSETS.does_asset_exist(MAP):
        LEVEL.load_level(MAP)   # it is the startup map, so it can't be deleted and recreated: empty it instead
        ACTORS.destroy_actors(ACTORS.get_all_level_actors())
    elif not LEVEL.new_level(MAP, False):
        raise RuntimeError("could not create " + MAP)

    # the ground, cones and city are spawned by AFSLapVehiclePawn: which ones show depends on the mode, and what you see is what counts as a hit

    sun = ACTORS.spawn_actor_from_class(ue.DirectionalLight, ue.Vector(0, 0, 2000), ue.Rotator(0, -40, 35))
    sun_light = sun.get_component_by_class(ue.DirectionalLightComponent)
    sun_light.set_editor_property("mobility", ue.ComponentMobility.MOVABLE)
    sun_light.set_editor_property("atmosphere_sun_light", True)
    sky = ACTORS.spawn_actor_from_class(ue.SkyLight, ue.Vector(0, 0, 1000), ue.Rotator())
    sky_light = sky.get_component_by_class(ue.SkyLightComponent)
    sky_light.set_editor_property("mobility", ue.ComponentMobility.MOVABLE)
    sky_light.set_editor_property("real_time_capture", True)
    ACTORS.spawn_actor_from_class(ue.SkyAtmosphere, ue.Vector(0, 0, 0), ue.Rotator())
    fog = ACTORS.spawn_actor_from_class(ue.ExponentialHeightFog, ue.Vector(0, 0, 0), ue.Rotator())
    fog_component = fog.get_component_by_class(ue.ExponentialHeightFogComponent)
    fog_component.set_editor_property("fog_density", 0.004)
    fog_component.set_editor_property("fog_height_falloff", 0.22)
    fog_component.set_editor_property("fog_max_opacity", 0.7)
    fog_component.set_editor_property("start_distance", 1200.0)
    fog_component.set_editor_property("directional_inscattering_exponent", 4.0)
    fog_component.set_editor_property("directional_inscattering_start_distance", 2500.0)
    ACTORS.spawn_actor_from_class(ue.PlayerStart, ue.Vector(0, 0, 100), ue.Rotator())

    LEVEL.save_current_level()
    ASSETS.save_directory(FOLDER)
    print("FSLAP_DRIVING_SLICE_OK " + MAP)


try:
    main()
except Exception as error:
    ue.log_error("FSLAP_DRIVING_SLICE_FAILED " + repr(error))
    raise
finally:
    ue.SystemLibrary.quit_editor()
