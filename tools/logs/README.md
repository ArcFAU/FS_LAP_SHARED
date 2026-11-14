# The simulator against the car's logs

These tools read the CAN logs of FAUmax τ's test days and compare what the car did with what FS Lap's two vehicle models do: FS Lap's own single-track model and the team's ADTM.

**First result (2026-11-14, seven logs):** the simulator matches the car on the skidpad and in top speed. It has more power and torque than the car showed, and it launches and brakes harder than any log does. The high-speed grip (downforce) could not be checked, because no log has the car at the limit in a fast corner.

## How to run it

1. Have the logs at `D:\SVN\logs\2026\02_Tau` and the team's DBC files at `D:\VD\Tools\homolog\config`. Both are only read. The paths are at the top of `validate.py` and `asc_decode.py`.
2. Build the simulator runs once: `tools\logs\sim_accel.bat` (needs Visual Studio's C++ compiler, not Unreal).
3. Write the AI lap traces once:
   - `data\physics_test\ailap.exe data\logs\sim_fslap.csv`
   - `data\physics_test\adtm\adtmlap.exe data\logs\sim_adtm.csv`
   - (`tools\physics_test\run.bat` and `adtm.bat` build these two programs.)
4. `python tools\logs\validate.py`

It prints the tables below and writes them to `data\logs\validation.md`. The first run decodes each log (about 20 s per 300 MB) into `data\logs\<name>.npz`; later runs take a few seconds. Python needs only NumPy.

| File | What it does |
|---|---|
| `asc_decode.py` | Decodes one Vector `.asc` log with the DBC into a NumPy file: IMU, steering angle, brake pressures, pedal, the four inverters, DC link and battery current. `python tools\logs\asc_decode.py <log.asc>` |
| `sim_accel.cpp`, `.bat` | The simulator's side: a standing start and full braking, circles at fixed steering angles (the grip limit against speed) and the AI's lap, for both vehicle models. `sim_accel.exe [Pmax_kW Tm_Nm]` |
| `validate.py` | Finds the standing starts, limits and laps in the logs and prints them next to the simulator's numbers. |

## What the logs contain

- **Speed** comes from the four motor speeds (gear 12.421, tyre radius 0.205 m). The front wheel speed sensors read zero in every log looked at, and there is no GPS.
- **Check of that speed:** on the skidpad, lateral acceleration = yaw rate × speed holds within 0.3 %. Under power the driven wheels slip, so the speed reads up to 4 % high there (FSG endurance: 0.958).
- **Motors 1 and 2 are the rear axle, 3 and 4 the front.** In a launch the front pair spins up while the rear pair follows the car.
- **`FSG\AccelFSG.asc` has no run in it:** the car stands still for 64 s.
- **Inverter data rate:** 100 Hz on the test days, 10 Hz in the FSG endurance log, 31 Hz in "TC funktioniert".

## Findings

### 1. Powertrain: the simulator has more power and torque than the car showed

| | Car (logs) | Simulator (stock setup) |
|---|---|---|
| Largest motor torque | 18.8–18.9 Nm on every full-power day | 21 Nm |
| Power at full throttle | 64–67 kW at the motor shafts, 70–74 kW electrical | 80 kW limit, 77.6 kW at the wheels |
| Top speed | 123.3 km/h (10-18 freestyle); 93 km/h on days with a limiter | 122.6 km/h (FS Lap model), 118.5 km/h (ADTM) |
| FSG endurance | 10.4 Nm and 32 kW at most | |

The top speed agrees. Torque is 10 % lower on the car and power about 17 % lower. Both are setup rows in FS Lap (**System power limit**, **Motor torque, each of 4**), so the as-logged car can be driven today; on the ADTM only the power limit acts, because its motor torque is fixed when its code is generated.

### 2. Standing starts: the car is slower off the line

| | 0–60 km/h | 0–90 km/h | Largest acceleration |
|---|---|---|---|
| Car, 5 flat-out starts (09-19 and 10-18) | 1.45–1.81 s | 2.68–2.92 s | 10.6–12.0 m/s² |
| Simulator, FS Lap model, stock | 1.34 s | 2.01 s | 15.2 m/s² |
| Simulator, ADTM, stock | 1.27 s | 1.94 s | 14.7 m/s² |
| Simulator, FS Lap model, as logged (66 kW, 18.9 Nm) | 1.35 s | 2.18 s | 14.4 m/s² |
| Simulator, ADTM, 66 kW | 1.28 s | 2.11 s | 14.3 m/s² |

In all five starts the front wheels ran 48–178 % faster than the rear, so the traction control was not holding the fronts and the car was not using its full grip. With the logged power and torque the simulator is still 0.1–0.5 s quicker to 60 km/h and 0.5–0.8 s quicker to 90 km/h. No log reaches 75 m flat out, so the acceleration event's time cannot be compared.

### 3. Skidpad: agrees

| | Lateral acceleration at 40–50 km/h |
|---|---|
| Car, skidpad log (10-18) | 16.3–16.6 m/s² |
| Simulator grip limit, ADTM | 16.6 m/s² |
| Simulator grip limit, FS Lap model | 15.2 m/s² |

The ADTM matches the car. FS Lap's own model has about 8 % less grip at that speed.

### 4. Grip at higher speed (downforce): not confirmed by the logs

| Speed | Car, most used in any log | Simulator limit, FS Lap model | Simulator limit, ADTM |
|---|---|---|---|
| 30–40 km/h | 15.9 m/s² | 14.1 | 15.0 |
| 40–50 km/h | 16.6 | 15.2 | 16.6 |
| 50–60 km/h | 13.7 | 16.4 | 18.5 |
| 60–70 km/h | 12.1 | 17.6 | 19.5 |
| 70–80 km/h | 11.6 | 19.2 | 21.1 |

The simulator's grip rises with speed, from its downforce (ClA 5.4). The logs show the opposite, but that only says the drivers did not corner at the limit above 50 km/h in these logs; what the car can do there is not in the data. A constant-radius test at 60–80 km/h, or a fast sweeper driven at the limit, would settle it. The table leaves out the "freestyle" log's 70–80 km/h value (21.5 m/s²), which comes from sliding, not cornering.

### 5. Braking: the logs never reach the limit

- The car's hardest sustained braking is 10–13 m/s² (99 % of the braking above 30 km/h; 13.4 in the FSG endurance).
- The deceleration rises in proportion to the brake pressure, up to the highest pressures logged, with no plateau. So these stops were below the grip limit.
- The simulator brakes at 19 m/s² (FS Lap model) and 26 m/s² (ADTM) on average from top speed, and its AI uses 13.7–14.2 m/s² on a lap.
- The front brake pressure sensor reads 9–12 bar with the brake released on the test days (4 bar on the skidpad day's last log, 1 bar at FSG).

### 6. FSG endurance: the layout differs, the pace is close

- The log has one stint: 449 s, about 4.7 laps, then the car stops.
- Lap times from one full turn of heading to the next: 92.0 s, 85.3 s, 96.6 s.
- The lap is about 1.17–1.21 km long. The simulator's FSG track is 1103 m, and its corner sequence matches the logged one only loosely (correlation 0.54 after stretching). The simulator's track is not the 2026 layout.
- Mean lap speed: 47.8 km/h logged (a little less in truth, see the speed check above). The pedal never went above 73 %.
- The simulator's AI at the endurance's power (32 kW, 10.4 Nm) laps its own track in 80.55 s, a mean of 49.3 km/h. At stock power it takes 77.40 s (51.3 km/h).

So on a similar track at the same power, the simulator's AI is about 3 % faster than the endurance driver. That is a plausible gap between an ideal driver and an endurance stint, but it is not a like-for-like lap.

### 7. Steering: the car's ratio is a little quicker than the simulator's

Measured once by hand from the logs (not yet part of `validate.py`): at 3–8 m/s and under 4 m/s² lateral, the road wheel angle is wheelbase × yaw rate / speed, and the logged steering wheel angle follows it with a correlation of 0.993–0.999.

| | Steering wheel degrees per road wheel degree | Steering wheel lock |
|---|---|---|
| Car: skidpad, "TC funktioniert", freestyle (10-18) | 3.90–3.96 | ±100–102° is the most in any of the three |
| Car: FSG endurance | 4.09 | ±82° used |
| Simulator, ADTM | 4.25 | ±110° |
| Simulator, FS Lap model | 4.38 (110° for 25° at the road) | ±110° |

- The logged steering angle is positive to the right.
- On the skidpad (radius 8.75 m as driven) the steering wheel angle grows from 41° at 5 m/s² to 54° at 17 m/s²: about 1.1° per m/s² at the wheel, 0.29° per m/s² at the road. That is the car's understeer gradient there; the simulator's has not been measured the same way yet.

## What this says about the simulator

1. **Use the logged power and torque as the stock car,** or keep the current values as the design target and add an "as logged" preset. That is the team's choice.
2. **The ADTM's low-speed grip is right.** FS Lap's own model could get 8 % more.
3. **Launch grip is too high** in both models, or the car's traction control costs that much. A start with working traction control would tell which.
4. **Downforce and limit braking are unverified.** They need test runs that reach the limit.
5. **Steering:** the car needs 8-10 % less steering wheel angle than the simulator for the same corner, and its lock looks like ±100°, not ±110°. FS Lap's 1:1 steering (setup row **Wheel rotation**) uses ±110°. Check the rack travel in the CAD before changing it.

## Not done yet

- **The real FSG 2026 track.** The log has speed and yaw rate, so the driven line can be rebuilt and used as a track in the simulator. Then the simulator and the car can be compared corner by corner.
- **The other logs.** About 60 more logs are in the folder (Höffner test days, FSF autocross and endurance, the "Fahrwerk" day). Add them to `LOGS` in `validate.py`.
- **Steering in `validate.py`.** The ratio and the skidpad's understeer gradient (finding 7) are not in the script yet, and the simulator's understeer gradient is not measured.
- **Ride height and spring travel** are logged at the front; they could check the aero load directly.
- **Energy use** per lap from the battery current.
