# FS Lap — driver-in-the-loop simulator for the FAUmax

FS Lap is High Voltage Motorsports' driver-in-the-loop (DIL) simulator, built in Unreal Engine 5 for the vehicle dynamics team. You drive the FAUmax τ (2026) on the FSG endurance track, an open paddock or a city circuit, with a keyboard, a gamepad or a steering wheel such as the Moza R3. A 600 Hz vehicle model runs underneath. It is checked against the team's lap time simulation (LTS).

This repository is the team's shared copy. It is updated from the development repository each time work is committed there. This document is updated with it.

> Synced from development commit `7dd64a6` on 2026-11-13.

---

## Contents

1. [What you can do with it](#1-what-you-can-do-with-it)
2. [Getting started](#2-getting-started)
3. [Driving](#3-driving)
4. [The vehicle model](#4-the-vehicle-model)
   - [4a. The complete LTS and lap data](#4a-the-complete-lts-and-lap-data)
   - [4b. Driving the team's ADTM](#4b-driving-the-teams-adtm)
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
- **The complete LTS as the reference:** the team's lap time simulation (MATLAB) computes the reference lap for your current setup, with the point-mass, single-track or dual-track model (section 4a).
- **Lap data:** every completed lap is saved at 600 Hz (CSV) and in the LTS run format, and `tools/lts/compareLaps.m` compares it with an LTS lap or another driven lap (section 4a).
- **The team's ADTM as the car:** drive the team's Advanced Dual Track Model live instead of FS Lap's own model, compiled into the game (section 4b).

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
3. Follow the four steps, pressing **Enter** after each: hands off the wheel and feet off the pedals; turn the wheel right as far as you want full steering lock; press the throttle fully; press the brake fully. **No pedals connected:** press **Enter** on the throttle and brake steps without pressing anything; then **W / S** (or a gamepad's triggers) are throttle and brake while the wheel steers.
4. **Esc** goes back. The setup is saved and used from then on. The keyboard and gamepad keep working and win while they are pressed.

The wheel's rotation range is set in the wheel's own software (Moza Pit House). The wheel's buttons are not used yet.

**Force feedback (ADTM only):** setup row **Force feedback**, off by default.
- **What you feel:** the ADTM's steering torque (`T_Steering`, section 4b) while you drive. There is no force in menus, the pause menu, replays, with the autopilot, or on FS Lap's own model.
- **Strength:** **Right** raises it in steps of 10 %. At 100 %, the largest torque on the ADTM's AI hot lap (25.5 Nm at the steering wheel) is the wheel's strongest force; the wheel's own maximum is set in Pit House. Changes in force are smoothed: full force takes 0.2 s to build.
- **First try:** hold the wheel and start at 10 %. In a corner the wheel must pull back towards the centre. If it pulls further into the corner, go back to off and press **Left** once more: `10 %, reversed`. Wheel drivers do not all push the same way.
- **Exclusive access:** while force feedback is on, FS Lap takes the wheel exclusively, so other programs cannot send it effects. A controller without force feedback stays shared, and the row says so.
- **Untested on the R3:** force feedback was built and tested without a force feedback wheel.

## 4. The vehicle model

- **Model:** a single-track (bicycle) model at 600 Hz with load transfer, a tyre grip envelope (peak μ at 600 N, load sensitivity, peak slip angle), aero downforce and drag with an aero balance, four motors with a system power limit, brake balance, roll-stiffness distribution and torque vectoring. In Hot lap, driver assists (brake, traction and steering help) can be switched on or off in the setup; laps with and without them are kept apart.
- **Where it comes from:** it is a port of the team's HoMo-LAP lap simulator (web, MATLAB and Simulink versions). The C++ core in `FSLapUE/Plugins/HoMoLapPhysics` has no Unreal types and builds on its own (`tools/physics_test`).
- **Check:** the built-in AI driver laps the FSG endurance track in **77.405 s** with the stock car. This is checked on every change (`tools/validate.ps1`). The stored LTS reference lap is 75.06 s and is shown in the HUD. The complete LTS (single-track) gives 74.952 s for the stock car (section 4a).
- **Data:** vehicle parameters of the τ (vehicle-params), the LTS vehicle file, and CFD aero packages.

**Setup parameters (setup screen):** aero package, downforce ClA, drag CdA, aero balance, car mass, driver mass, front weight share, CoG height, system power limit, motor torque (each of 4), gear ratio, peak μ at 600 N, load sensitivity exponent, peak slip angle, brake balance, maximum brake force, front roll-stiffness share, torque-vectoring authority, steering lock, and the driver assists. **R** restores the stock car.

**Aero packages:** Tau 2026 (LTS vehicle file: ClA 5.40, CdA 1.70, 50 % front); Tau 2026 (vehicle-params: 4.90, 1.40, 55 %); Upsilon 2027 CFD baseline 1011 (5.64, 1.90, 50 %); Upsilon 2027 CFD best V4_K (6.94, 1.96, 45 %); Tau 2026 without downforce.

**Car geometry in the model:** wheelbase 1.53 m, track 1.24 m front / 1.20 m rear, CoG height 0.293 m, 50 % on the front axle, wheel radius 0.202 m. These match the CAD (section 6).

### 4a. The complete LTS and lap data

**What the LTS is here.** The team's LTS (`laptimesimulation`) is a quasi-steady-state solver: it computes the fastest speed profile for a track from the car's limits. It cannot be driven. FS Lap uses it as the **reference lap**: the HUD gap, the sector deltas and the LTS ghost car. The game's own 600 Hz model stays the one you drive.

**Using it:**
1. Menu → **SETUP** → row **LTS reference**. **Left / Right** chooses `stored lap` (the built-in 75.06 s lap, the default), `point mass`, `single track` or `dual track`.
2. When you leave the setup, FS Lap sends the current setup to the LTS as a job. The row shows `starting MATLAB … s`, then `running … s`, then the lap time. The HUD and the ghost switch to the new lap when it is ready.
3. A job takes about 1 minute, plus about 45 s the first time while MATLAB starts. MATLAB keeps running in the background for 30 minutes after the last job. Results are cached per model and setup, so a setup you used before is instant.

**What is passed to the LTS:** driver and car mass, CoG height, front mass share, ClA, CdA, aero balance, tyre μ and load sensitivity, front roll-moment share, system power, motor torque, gear ratio and brake balance. Everything else comes from the LTS's own vehicle file (`vehicles/`).

**What you need:** MATLAB (R2024b tested) with the Optimization Toolbox, and a checkout of `laptimesimulation` that has the job interface (`+lts/+viewer/worker.m`). FS Lap finds them through two environment variables:

| Variable | Default |
|---|---|
| `FSLAP_LTS_ROOT` | `D:\VD\laptimesimulation` |
| `FSLAP_MATLAB` | the newest `C:\Program Files\MATLAB\R20*\bin\matlab.exe` |

FS Lap only adds the LTS folders to the MATLAB path; it does not change the checkout. Jobs and results are in `FSLapUE\Saved\LTS\` (`queue\`, `runs\<job>\case_001.json`). If a job fails, the row says so and names the log file.

**Lap data.** Every lap completed in Hot lap, Arcade or City is saved to `FSLapUE\Saved\Runs\<date>_<time>_<mode>\` as two files. Laps that are not completed are dropped.
- **`lap_NN.csv`:** every simulation step (600 Hz). Columns: `time_s, distance_m, x_m, y_m, heading_rad, vx_mps, vy_mps, speed_mps, yawRate_radps, ax_mps2, ay_mps2, steerInput, steer, roadWheelAngle_rad, throttle, brake, handbrake, power_kW, lateralOffset_m, slipFront, slipRear, grass`. `steerInput` is the driver's input and `steer` is what the car does with it, after the assists.
- **`lap_NN.run.json`:** the same lap at 100 Hz in the LTS run format (`homo-lts.run.v1`), so the team's LTS tools can read it. `lap.curvature_1pm` and `lap.lateralAcceleration_mps2` follow the LTS sign convention, which is opposite to the game's. The block `fslap` adds the mode, the lap result (time, penalties, sectors), the setup, the reference in use and all channels in the game's own signs.

**Comparing laps in MATLAB:**

```matlab
addpath('tools/lts')
c = compareLaps('FSLapUE/Saved/Runs/<session>/lap_01.run.json', 'FSLapUE/Saved/LTS/runs/<job>/case_001.json', SavePng="compare.png");
```

It plots speed, the time gap, lateral acceleration and the driver's inputs over distance, and prints lap and sector times. Either file can be a driven lap or an LTS lap. The second lap's distance is scaled to the first lap's track length, because the LTS track and the game's centreline differ by a few metres. For the stock car, the AI lap is 2.45 s slower than the single-track LTS lap (77.405 s against 74.952 s). The point-mass LTS gives 71.037 s and the dual-track LTS 77.817 s.

**Offline co-simulation with the Advanced Dual Track Model (ADTM):**

```matlab
addpath('tools/lts')
r = adtmReplay('FSLapUE/Saved/Runs/<session>/lap_01.run.json', SavePng="adtm.png");
```

It drives the logged lap again with the team's ADTM (40 states, TMeasy tyres, double-wishbone kinematics), open loop: the driver's inputs are replayed and nobody corrects the car, as when the team replays real-car logs. It plots both cars over time (speed, yaw rate, lateral acceleration, front wheel angle) and their paths. It prints the RMS differences and writes `lap_NN.adtm_input.csv` in the ADTM's input format, which the ADTM's own start script can load.

How the inputs are converted:

| FS Lap | ADTM |
|---|---|
| road wheel angle | steering wheel angle `delta_D`, from the ADTM's own steering kinematics, so the front wheels turn as in FS Lap |
| brake pedal × `Fb`, split by `brakeFront` | brake pressures `p_BFA` / `p_BRA` (bar), through the ADTM's piston area, disc radius and pad friction |
| throttle pedal | `throttle`, capped at FS Lap's system power (the ADTM limits each motor, not the 80 kW system) and zero while braking |
| aero package (ClA, CdA, balance) | copied into the ADTM vehicle |

Everything else (mass, tyres, suspension) comes from the ADTM's vehicle file (`FAUmaxTau.m` by default). The road friction scaling defaults to `default_mu0.7`, as in the team's ADTM setups. Options: `AdtmRoot`, `Vehicle`, `EnvConditions`, `SimFreq`. The ADTM folder comes from `FSLAP_ADTM_ROOT` (default `D:\FSLap_VD\advanced-dual-track-model`); its MATLAB project is opened, nothing in it is changed.

For the stock AI lap, a 77 s lap takes about 1 minute. The speed differs by 0.61 m/s RMS and the yaw rate by 0.12 rad/s RMS. The ADTM covers 1075 m against FS Lap's 1103 m, and its path drifts away from the track, as any open-loop replay does.

**Closed loop:** `adtmReplay(..., Mode="closed")` has a driver model drive the ADTM along the path driven in FS Lap. At each step it takes FS Lap's steering and pedals at the same distance, then corrects them:
- **Steering:** for the heading error, and for the lateral error (Stanley steering);
- **Pedals:** towards FS Lap's speed at that distance.

It compares both cars over distance:
- speed;
- time gained or lost;
- how far the ADTM is off the path;
- the front wheel angle its driver needed, against FS Lap's (more angle means the ADTM car understeers more);
- the paths.

It stops if the car is more than 5 m off the path. For the stock AI lap: the ADTM takes 77.630 s against FS Lap's 77.400 s to the last logged point. It is at most 1.51 m off the path (in the fast section around 420–560 m), and it needs slightly more steering. The run takes about 1 minute. It does not write the ADTM input file, because the inputs depend on the ADTM's own response.

### 4b. Driving the team's ADTM

You can drive the team's Advanced Dual Track Model (ADTM) itself, live, with the wheel, instead of FS Lap's own model.

**Using it:** Menu → **SETUP** → row **Vehicle model** → **Left / Right** switches between `FS Lap (single track)` and `ADTM (team, dual track)`.
- **Modes:** it applies in hot lap and free roam. Arcade and city keep FS Lap's own model, because their handling layer belongs to FS Lap.
- **Records:** laps on the ADTM keep their own best laps and ghosts.
- **Lap files:** `calculationKernel` names the ADTM and its source.

**How it works:**
- **Compiled into the game:** the ADTM's MATLAB code (`f_vehicleModel`: 40 states, TMeasy tyres, double-wishbone kinematics) is turned into C++ by MATLAB Coder (`tools/adtm/adtm_codegen.m`, into `Plugins/HoMoLapPhysics/Source/HoMoLapPhysics/Private/ADTM`). The game steps it with the ADTM's own Runge-Kutta scheme at 600 Hz.
- **No MATLAB needed while driving.** One step takes about 40–60 µs, about 4 % of one CPU core.
- **What FS Lap still does:** it reads the wheel, pedals and keys exactly as before (pedal ramps, driver assists, steering lock), and it keeps the lap rules, cones, penalties, timing and the world.
- **What the ADTM does:** it moves the car.
- **Input conversion,** the same as in `adtmReplay.m` (section 4a):
  - the road wheel angle becomes the ADTM steering wheel angle through the ADTM's own steering kinematics;
  - the brake becomes front and rear pressures through `Fb` and `brakeFront`;
  - the throttle is capped at the system power;
  - grass sets the road friction to 0.62.
- **Moving the car:** when the game moves the car (restart, back on track, a wall), the ADTM starts again from there, settled on its springs.

**What comes from where:**
- **From FS Lap's setup:** aero (ClA, CdA, balance), brake force and split, and system power.
- **From the ADTM vehicle file baked in at generation:** everything else, including mass, tyres, suspension and motors. Today that is `FAUmaxTau.m` with `default_mu0.7`.
- **Not used by the ADTM:** FS Lap's mass, tyre, roll and torque-vectoring rows.
- **No handbrake:** the handbrake does nothing on the ADTM, as on the real car.
- **Force feedback:** from the ADTM's `T_Steering` (section 3). On the AI hot lap it peaks at 25.5 Nm and pulls the wheel back towards the centre 89 % of the steered time.

**Results with the stock car:**

| | FS Lap's own model | ADTM |
|---|---|---|
| AI hot lap | 77.405 s | 77.620 s, valid, 0.56 m at most off the centreline |
| 0–100 km/h | 1.577 s (arcade free roam) | 2.19 s |
| 75 m | 3.075 s | 3.54 s |
| Top speed | | 118 km/h, the motor speed limit |
| Frame rate in game | 74–75 fps | 73.7 fps |

The ADTM's 77.620 s matches the offline closed-loop replay (77.630 s).

**After the team changes the ADTM or its vehicle file:**
1. In MATLAB (needs MATLAB Coder): `addpath('tools/adtm'); adtm_codegen` (options: `AdtmRoot`, `Vehicle`, `EnvConditions`).
2. Check it against the MATLAB code: `adtm_codegen(Mex=true); adtm_check`. On the stock data, compiled and MATLAB agree to 1.9e-14.
3. Build the game and run `tools\physics_test\adtm.bat`.

Without the generated folder, FS Lap builds as before and the row says `ADTM not built yet`. The generated code carries MathWorks' sponsored-licence notice.

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
  Source/FSLapUE/                 game flow, car presentation, HUD, wheel input (FSLapWheel), LTS jobs (FSLapLts), lap logging (FSLapLog), tests (Dev/)
  Plugins/HoMoLapPhysics/         the vehicle model, track data, lap rules and AI driver (plain C++); the ADTM wrapper (HoMoLapAdtm) and its generated code (Private/ADTM)
  Content/FS/Generated/           imported assets: car, CAD parts, track, city, sound, materials (Git LFS)
tools/                            conversion, import and test scripts
  cad/                            NX Open and Blender scripts for the CAD export (README inside)
  physics_test/                   stand-alone build and test of the vehicle model (adtm.bat: the ADTM)
  adtm/                           MATLAB Coder generation of the ADTM for the game (adtm_codegen.m) and its check (adtm_check.m)
  lts/                            MATLAB: compareLaps.m (driven laps against LTS laps), adtmReplay.m (driven laps through the ADTM)
vehicle-data/                     suspension hardpoints and design table
Play FS Lap.bat                   starts the game full screen
```

The main classes:
- **`AFSLapVehiclePawn`:** runs the 600 Hz simulation, input, menus, cameras, sound, effects, replay and saving best laps.
- **`AFSLapHUD`:** draws the HUD and menus.
- **`FFSLapWheel`:** reads DirectInput wheels. It searches for them on a worker thread.
- **`FFSLapLts`:** sends LTS jobs to a MATLAB worker through a file queue and reads the results.
- **`FFSLapLog`:** records each lap and writes the CSV and run files on a worker thread.
- **`HoMoLap::Simulation`:** the vehicle model and lap rules. `Simulation::external` lets another vehicle model move the car.
- **`HoMoLap::MakeAdtm`:** the team's ADTM as that model.

## 8. Tests

All tests run from PowerShell in the repository root. Close the Unreal Editor first.

| Command | What it checks |
|---|---|
| `tools\validate.ps1` | Builds the vehicle model alone and runs the AI in sim, arcade, free roam and city mode. The lap times must match (sim 77.405 s, arcade 75.367 s, city 89.633 s) with no wall contacts. No Unreal needed. |
| `tools\validate.ps1 -Game` | The above, plus the in-game tests below |
| `tools\slice_test.ps1 -Mode sim` (or `arcade`, `city`, `free`) | Starts the real game, lets the AI drive a lap and takes photos from around the car. Results in `FSLapUE\Saved\SliceTest\<mode>\results.txt`: lap times, fps, average and 95th-percentile frame time, the longest frame and the number of frames over 100 ms (hitches). |
| `tools\slice_test.ps1 -Mode keys` | Presses every key through the game and checks what each did (48 steps). Each line also reports the frame rate and the car's speed. |
| `tools\slice_test.ps1 -Mode lts` | Has the real LTS compute the single-track reference for the stock car (needs MATLAB, section 4a), then lets the AI drive a hot lap against it. Checks the reference (`reference=LTS 1-track 74.952`) and the lap files (`lap_log=`). |
| `tools\physics_test\adtm.bat` | The ADTM in the game's core without Unreal: the AI laps the FSG track (77.620 s), the car is moved and must carry on, and a free-roam standing start (0–100 km/h in 2.185 s, with the assists on). The steering torque must pull back towards the centre most of the time. It must run faster than real time. Needs the generated code. |
| `tools\slice_test.ps1 -Mode adtm` | The in-game hot lap with the ADTM as the vehicle model (`vehicle_model=ADTM ...`). Add `-Adtm` to any other mode to run it with the ADTM. |

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

**Not there yet:** force feedback on FS Lap's own model (it has none; only the ADTM gives it), a four-wheel model with suspension kinematics (the hardpoints are ready for it), a Pacejka tyre model, real-car data overlays, the wheel's buttons, and a field of view set from the rig's screen size. The LTS reference covers the FSG endurance track only, and the lap files are not yet in .mat format.

**Proposed next, in order, pending the team's answers:**
1. **Force feedback, tried on the Moza R3:** check the direction and the strength with a driver (section 3).
2. **Rig basics:** the real steering ratio from the CAD rack, so the wheel turns the ADTM's steering directly instead of through FS Lap's speed-dependent lock, and a field of view from the screen geometry.
3. **The ADTM's vehicle data from FS Lap's setup:** mass and tyres editable in the setup screen, not only at generation.
4. **Better vehicle model:** a four-wheel model and the tyre model, or live co-simulation with the team's Simulink model.
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

- **2026-11-13 (night, later):** the wheel setup accepts a wheel without pedals: Enter on the throttle and brake steps with nothing pressed, then the keys or a pad drive the pedals.
- **2026-11-13 (night):** force feedback from the ADTM's steering torque, setup row **Force feedback** (section 3). Off by default; not yet tried on a force feedback wheel.
- **2026-11-13 (evening):** drive the team's ADTM (section 4b). It is compiled into the game with MATLAB Coder and chosen in the setup row **Vehicle model** (hot lap and free roam). New tests: `tools\physics_test\adtm.bat`, slice test mode `adtm` and the `-Adtm` switch.
- **2026-11-13 (later):** the complete LTS and lap data (section 4a).
  - **LTS reference:** the setup row **LTS reference** has the team's LTS compute the reference lap for the current setup (point-mass, single-track or dual-track model).
  - **Lap logging:** every completed lap is saved as a 600 Hz CSV and an LTS run file.
  - **`tools/lts/compareLaps.m`:** compares driven and LTS laps.
  - **Test mode `lts`.**
  - **`tools/lts/adtmReplay.m`:** offline co-simulation through the team's Advanced Dual Track Model. It replays a driven lap open loop, or drives the ADTM along the driven path with a driver model (`Mode="closed"`).
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
