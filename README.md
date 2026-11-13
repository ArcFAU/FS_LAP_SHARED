# FS Lap — driver-in-the-loop simulator for the FAUmax

FS Lap is High Voltage Motorsports' driver-in-the-loop (DIL) simulator, built in Unreal Engine 5 for the vehicle dynamics team. You drive the FAUmax τ (2026) on the FSG endurance track, an open paddock or a city circuit, with a keyboard, a gamepad or a steering wheel such as the Moza R3. A 600 Hz vehicle model runs underneath. It is checked against the team's lap time simulation (LTS).

This repository is the team's shared copy. It is updated from the development repository each time work is committed there. This document is updated with it.

> Synced from development commit `1e61dc4` on 2026-11-13.

---

## Contents

1. [What you can do with it](#1-what-you-can-do-with-it)
2. [Getting started](#2-getting-started)
3. [Driving](#3-driving)
4. [The vehicle model](#4-the-vehicle-model)
5. [The car model and the CAD](#5-the-car-model-and-the-cad)
6. [Vehicle data in this repository](#6-vehicle-data-in-this-repository)
7. [Project layout](#7-project-layout)
8. [Tests](#8-tests)
9. [Rebuilding generated assets](#9-rebuilding-generated-assets)
10. [Limits and plans](#10-limits-and-plans)
11. [What is not in this repository](#11-what-is-not-in-this-repository)
12. [Change log](#12-change-log)

---

## 1. What you can do with it

- **Hot lap:** the car as the team's data describes it, on the FSG endurance track, with FS Rules penalties (cone down +2 s, off course +10 s, track cut = lap does not count), sector times, the gap to the LTS lap and to your own best lap, and ghost cars for both.
- **Arcade:** the same track with more grip and power and a forgiving handling layer. No penalties.
- **City:** a generated city about 3 km across with a timed circuit and moving traffic.
- **Free roam:** an open paddock with a skidpad and an acceleration strip. 0–100 km/h and 75 m are timed from every standing start.
- **Setup:** change the aero package, mass, power, tyres, brakes and chassis (18 parameters, section 4). Best laps are kept per mode and setup.
- **Replay:** watch your best lap from roadside cameras.

## 2. Getting started

### What you need

| | Minimum (tested) | Better |
|---|---|---|
| OS | Windows 10/11, 64-bit | |
| GPU | GTX 1650 4 GB (about 65–75 fps at 1080p) | RTX 3060 or better |
| RAM | 16 GB (32 GB tested) | 32 GB |
| Disk | about 70 GB: engine 50 GB, project and caches | SSD |

Software:
- **Unreal Engine 5.8.4** from the Epic Games Launcher.
- **Visual Studio 2022** (or Build Tools) with "Game development with C++", MSVC v14.50 and a Windows 10/11 SDK.
- **Git** with **Git LFS** (`git lfs install` once). The Unreal assets are stored in LFS.
- **Python 3.11 or newer**, only for the tools in `tools/`: `py -m pip install -r requirements-tools.txt`.

### Get it and build it

```powershell
git lfs install
git clone https://github.com/ArcFAU/FS_LAP_SHARED.git
cd FS_LAP_SHARED
# build the editor target once (replace the engine path with yours)
& "<UE>\Engine\Build\BatchFiles\Build.bat" FSLapUEEditor Win64 Development "-Project=$PWD\FSLapUE\FSLapUE.uproject" -WaitMutex
```

`<UE>` is your engine folder, for example `C:\Program Files\Epic Games\UE_5.8`. If Unreal asks which engine to use when you open `FSLapUE.uproject`, choose 5.8.

### Run it

- **Double-click `Play FS Lap.bat`.** It starts the game full screen. If your engine is not at `D:\UE\UE_5.8`, set the `UE_ROOT` environment variable to your engine folder first. The first start compiles shaders and takes several minutes; later starts take about a minute.
- **Or open `FSLapUE\FSLapUE.uproject`** in the Unreal Editor and press Play.

### Make a stand-alone build

```powershell
& "<UE>\Engine\Build\BatchFiles\RunUAT.bat" BuildCookRun "-project=$PWD\FSLapUE\FSLapUE.uproject" -noP4 -platform=Win64 -clientconfig=Development -build -cook -stage -pak -archive "-archivedirectory=$PWD\dist"
```

Copy the whole `dist\Windows` folder to the simulator PC and run `FSLapUE.exe`. That PC needs no engine.

## 3. Driving

| Action | Keyboard | Gamepad (Xbox layout) |
|---|---|---|
| Throttle / brake | W / S or Up / Down | right / left trigger |
| Steer | A / D or Left / Right | left stick |
| Handbrake | Space | A |
| Back on track (pause menu: restart) | R | |
| Camera: chase / onboard | C | |
| Pause | Esc or P | Start |
| Replay of the best lap (in the pause menu) | B | Y |
| Back to the menu (in the pause menu) | M | |
| Menu: choose / change / confirm | arrow keys, Enter | D-pad, A |

**Steering wheel (DirectInput wheels such as the Moza R3):**
1. Plug the wheel in. The game finds it within about 2 seconds, also while it is running.
2. Menu → **SETUP** → row **Steering wheel** → press **Right** to open the wheel setup.
3. Follow the four steps, pressing **Enter** after each: hands off the wheel and feet off the pedals; turn the wheel right as far as you want full steering lock; press the throttle fully; press the brake fully.
4. **Esc** goes back. The setup is saved and used from then on. The keyboard and gamepad keep working and win while they are pressed.

The wheel's rotation range is set in the wheel's own software (Moza Pit House). There is no force feedback yet, and the wheel's buttons are not used yet.

## 4. The vehicle model

- **Model:** a single-track (bicycle) model at 600 Hz with load transfer, a tyre grip envelope (peak μ at 600 N, load sensitivity, peak slip angle), aero downforce and drag with an aero balance, four motors with a system power limit, brake balance, roll-stiffness distribution and torque vectoring. In Hot lap, driver assists (brake, traction and steering help) can be switched on or off in the setup; laps with and without them are kept apart.
- **Where it comes from:** it is a port of the team's HoMo-LAP lap simulator (web, MATLAB and Simulink versions). The C++ core in `FSLapUE/Plugins/HoMoLapPhysics` has no Unreal types and builds on its own (`tools/physics_test`).
- **Check:** the built-in AI driver laps the FSG endurance track in **77.405 s** with the stock car. This is checked on every change (`tools/validate.ps1`). The LTS reference lap is 75.06 s and is shown in the HUD.
- **Data:** vehicle parameters of the τ (vehicle-params), the LTS vehicle file, and CFD aero packages.

**Setup parameters (setup screen):** aero package, downforce ClA, drag CdA, aero balance, car mass, driver mass, front weight share, CoG height, system power limit, motor torque (each of 4), gear ratio, peak μ at 600 N, load sensitivity exponent, peak slip angle, brake balance, maximum brake force, front roll-stiffness share, torque-vectoring authority, steering lock, and the driver assists. **R** restores the stock car.

**Aero packages:** Tau 2026 (LTS vehicle file: ClA 5.40, CdA 1.70, 50 % front); Tau 2026 (vehicle-params: 4.90, 1.40, 55 %); Upsilon 2027 CFD baseline 1011 (5.64, 1.90, 50 %); Upsilon 2027 CFD best V4_K (6.94, 1.96, 45 %); Tau 2026 without downforce.

**Car geometry in the model:** wheelbase 1.53 m, track 1.24 m front / 1.20 m rear, CoG height 0.293 m, 50 % on the front axle, wheel radius 0.202 m. These match the CAD (section 6).

## 5. The car model and the CAD

- **Bodywork, wings, undertray and wheels:** exported from the team's livery model in Blender (`tools/export_ue.py`, `tools/bake_livery_uv.py`) with the livery baked in.
- **Mechanical parts:** suspension (wishbones, pushrods, rockers, dampers, anti-roll bar), uprights, brakes, hubs, the AMK motors, main and front hoop, firewall, dash display and the team's steering wheel. They come from the 2026 NX model (`000_EV_CAD_Gesamtmodell`):
  1. Exported part by part to STL with NX Open, from a copy and never from the SVN checkout.
  2. Assembled in Blender with the assembly positions.
  3. Reduced to about 240,000 triangles and added to the game car (`tools/cad_to_ue.py`).
- **How they move:** the uprights, calipers and motors steer with the front wheels; the discs and hubs spin with the wheels; everything else is fixed to the body.
- **Steering wheel:** at its CAD position, 32.7 cm ahead of mid-wheelbase and 52 cm high, tilted −11.8°. It turns with the steering input.
- **Export rules:** the CAD export follows the assembly's MODEL reference sets. It leaves out bodies that are only linked copies of other parts, and it resets each part's working coordinate system before export. `tools/cad/README.md` explains the whole pipeline and its pitfalls.

## 6. Vehicle data in this repository

`vehicle-data/`:
- **`hardpoints.csv` / `.json`:** 34 suspension points of the 2026 car from the suspension skeleton (`S_0000_SKEL.prt`).
  - Points: wishbone inner and outer points, tie rods and toe links, pushrods, rocker pivots, dampers, rear anti-roll bar, wheel centres, contact patches and the CoG.
  - Frame: mm; x forward from the rear axle, y to the left, z up from the ground. The right side mirrors y.
  - The dampers lie across the car: the inner damper points are past the centre line.
- **`TAU_SKEL_Import.csv`:** the suspension design table that drives the skeleton. It holds wheelbase, half tracks, CoG, trail, scrub, caster, kingpin, camber, roll-centre heights, anti-pitch and anti-dive, pickup offsets, steering geometry, rocker and damper geometry, and tube diameters.

## 7. Project layout

```text
FSLapUE/                          Unreal project
  FSLapUE.uproject
  Config/                         engine, input and packaging settings
  Source/FSLapUE/                 game flow, car presentation, HUD, wheel input (FSLapWheel), tests (Dev/)
  Plugins/HoMoLapPhysics/         the vehicle model, track data, lap rules and AI driver (plain C++)
  Content/FS/Generated/           imported assets: car, CAD parts, track, city, sound, materials (Git LFS)
tools/                            conversion, import and test scripts
  cad/                            NX Open and Blender scripts for the CAD export (README inside)
  physics_test/                   stand-alone build and test of the vehicle model
vehicle-data/                     suspension hardpoints and design table
Play FS Lap.bat                   starts the game full screen
```

The main classes:
- **`AFSLapVehiclePawn`:** runs the 600 Hz simulation, input, menus, cameras, sound, effects, replay and saving best laps.
- **`AFSLapHUD`:** draws the HUD and menus.
- **`FFSLapWheel`:** reads DirectInput wheels. It searches for them on a worker thread.
- **`HoMoLap::Simulation`:** the vehicle model and lap rules.

## 8. Tests

All tests run from PowerShell in the repository root. Close the Unreal Editor first.

| Command | What it checks |
|---|---|
| `tools\validate.ps1` | Builds the vehicle model alone and runs the AI in sim, arcade, free roam and city mode. The lap times must match (sim 77.405 s, arcade 75.367 s, city 89.633 s) with no wall contacts. No Unreal needed. |
| `tools\validate.ps1 -Game` | The above, plus the in-game tests below |
| `tools\slice_test.ps1 -Mode sim` (or `arcade`, `city`, `free`) | Starts the real game, lets the AI drive a lap and takes photos from around the car. Results in `FSLapUE\Saved\SliceTest\<mode>\results.txt`: lap times, fps, average and 95th-percentile frame time, the longest frame and the number of frames over 100 ms (hitches). |
| `tools\slice_test.ps1 -Mode keys` | Presses every key through the game and checks what each did (46 steps). Each line also reports the frame rate and the car's speed. |

Add `-Engine <UE>` if your engine is not at `D:\UE\UE_5.8`.

## 9. Rebuilding generated assets

The assets in `FSLapUE/Content` are already built; you only need this to change them. The sources are not all in this repository.

| Asset | Tool | Needs |
|---|---|---|
| Body, wheels, livery | `tools/export_ue.py`, `tools/bake_livery_uv.py` | the team's livery model in Blender |
| CAD parts, steering wheel | `tools/cad/*` (NX), then `tools/cad_to_ue.py` (Blender), then `FSLAP_IMPORT=cad` | NX 2412 with a licence, a copy of the team CAD |
| Track | `tools/track_to_ue.py` | the FSG centreline from HoMo-LAP |
| City | `tools/city_to_ue.py` | nothing (generated) |
| Road textures | `tools/fetch_environment_assets.py` (CC0, Poly Haven) | internet |
| Sound | `tools/extract_audio.py`, `tools/make_sound_loops.py`, `tools/import_sound_ue.py` | the test-day videos |

The Unreal imports run through `tools\run_editor_script.ps1 import_ue.py FSLAP_IMPORT_OK`. Set `FSLAP_IMPORT` to `cad`, `city` or `environment` to import just that part.

## 10. Limits and plans

**Not there yet:** force feedback, a four-wheel model with suspension kinematics (the hardpoints are ready for it), a Pacejka tyre model, data logging, real-car data overlays, the wheel's buttons, and a field of view set from the rig's screen size.

**Proposed next, in order, pending the team's answers:**
1. **Data logging:** 600 Hz to CSV / .mat.
2. **Rig basics:** the real steering ratio from the CAD rack, and a field of view from the screen geometry.
3. **Force feedback** on the Moza R3.
4. **Better vehicle model:** a four-wheel model and the tyre model, or co-simulation with the team's Simulink model.
5. **VD tools:** standard test manoeuvres with automatic metrics, setup A/B blind tests, and real-car data overlays.

## 11. What is not in this repository

- **HoMo-LAP:** the web and MATLAB lap simulator.
- **Development notes and logs.**
- **The raw CAD:** neither the SVN model nor the STL exports.
- **The sound source videos.**
- **Local caches and builds:** `data/`, `dist/`, `Saved/`, `Intermediate/`.

Ask the simulator's maintainer if you need any of these.

## 12. Change log

Newest first.

- **2026-11-13:** FS Lap becomes a driver-in-the-loop simulator for the vehicle dynamics team.
  - **CAD parts:** the car's mechanical parts and the team's steering wheel come from the 2026 NX model.
  - **Driver figure:** removed.
  - **Freeze fix:** the game no longer freezes for half a second every 2 s while no wheel is attached (the wheel search runs on a worker thread).
  - **Visuals:** motion blur is off, so the spinning tyres no longer smear. Skid marks sit on the surface under them. The road lines no longer look lifted.
  - **Tests:** they report hitches.
- **2026-11-12:**
  - **Wheel and pad support:** steering wheels (DirectInput, with a wheel setup screen) and Xbox-style gamepads.
  - **Maps:** the real-city maps (Erlangen, Nürnberg) are removed; the maps are the FSG track, free roam and the generated city.
- **2026-11-08 to 11-11:** the Unreal project, the car with its livery, the vehicle model port (AI lap 77.405 s), HUD, menus, setup editor, ghosts, replay, sound, the generated city with traffic, and the packaged build.
