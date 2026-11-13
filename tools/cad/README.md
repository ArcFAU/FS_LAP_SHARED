# CAD tools: the team's car CAD, exported for FS Lap

These scripts live in `tools/cad` and expect their working folder at `D:\FSLap_CAD` (change the paths at the top of each script for another place). The notes below describe that working folder.

Made 2026-11-12. Everything here is derived from a **copy** of the SVN model. The SVN checkout (`D:\SVN\...`) is never opened for writing, and NX never saves a part here either.

- `Gesamtmodell\`: a copy of `D:\SVN\2026\1_Gesamtteam\000_EV_CAD_Gesamtmodell` (2026 car, FAUmax τ), 2,379 files, 2.9 GB, without `.svn`. Copied with `robocopy /E /XD .svn`. To refresh it, copy again; nothing here is edited.
- `tools\`: the scripts.
  - `dump_part.py`: an NX Open journal that writes one part's expressions, points, lines, sketches and features to JSON.
  - `dump_tree.py`: an NX Open journal that writes the assembly tree with every component's position in car coordinates.
  - `make_jobs.py`: the unique parts to export, each matched to its file in the copy. When a file name appears twice it takes the one outside the `alt*` (old version) folders. It skips standard fasteners, skeletons and the livery.
  - `export_stl.py`: an NX Open journal that exports each part's solid bodies to STL (0.5 mm chord) in the part's own frame. It can be resumed.
  - `hardpoints.py`: the named suspension points as `out\hardpoints.json` / `.csv`.
  - `assemble_car.py`: background Blender. It places every STL with the tree positions, renders check views with the hardpoints as red dots, and writes a GLB.
- `out\`: the results.
  - `hardpoints.csv` / `.json`: 34 suspension points, left side, in mm.
  - `TAU_SKEL_Import.csv`: the suspension design table.
  - `tree.json`, `skel.json`, `s_skel.json`, `jobs.json`.
  - `stl\`: one STL per part.
  - `preview\`: check renders and `faumax_tau_cad.glb`.

Run NX journals with `"C:\Program Files\Siemens\NX2412\NXBIN\run_journal.exe" <journal.py> -args ...`. Each run takes one network licence from `siemens-lizenzen.voltages.de`.

## What was found

- **Car frame:** mm, x forward from the rear axle, y to the left, z up from the ground (ISO 8855). Rear wheel centre (0, ±600, 200), front wheel centre (1530, ±620, 200), CoG (765, 0, 290).
- **These match the games:** wheelbase 1.53 m, track 1.24 / 1.20 m, CoG height 0.290 m (game 0.293 m), 50 % on the front, wheel centre 200 mm (game radius 0.202 m).
- **Suspension:** double wishbones front and rear, pushrod to rocker. Rocker pivots FRP/RRP. The dampers lie across the car: the inner damper points FDI/RDI are past the centre line. Rear anti-roll bar points RRARB/RTARB/RLARB. Front tie rod and rear toe link. Tubes are 16/12 mm (wishbones), 20/16 mm (pushrods) and 14/10 mm (tie rods).
- **Assembly:** 2,401 components and 928 unique parts to export. 7 component names have no file in the model: the aero top-plate flaps, `C_FW06_BondingSupport`, `C_MO07_Lapbelt_insert` and `S_RA03_IgusCustomBearing`.

## Full car (2026-11-12)

- **Export:** all 924 parts in 6 min, 742 with geometry, no errors. 211 MB of STL.
- **Assembly:** `assemble_car.py` placed 1,608 instances. The renders in `out\preview\` show the whole car in place: monocoque, nose, wings, undertray, sidepods, suspension, steering, wheels, roll hoops and electronics.
- **To clean up before any game use:**
  - the FS rule templates (`C_TP00_Templates`, e.g. the driver head sphere);
  - a few parts that float off the car (a panel under the rear, small brackets);
  - the size: `faumax_tau_cad.glb` is 285 MB, so it needs decimation or a reduced part list.

## Export fixes (2026-11-12, after the owner saw parts flying around)

- **Moved WCS:** NX's STL export writes coordinates relative to the part's WCS, and 10 parts were saved with a moved WCS (side wings, pushrod flanges, steering gearbox ...). `export_stl.py` now puts the WCS back on the absolute system before exporting (in memory only, never saved).
- **Hidden bodies:** only the bodies of the part's MODEL reference set are exported, the set the assembly shows. A part without one gets its visible bodies. This drops 8,107 hidden bodies in 45 parts: sheet-metal flat patterns, simplified boards, construction and envelope bodies.
- **Surface bodies (owner: the bottom suspension cover was missing):** `A_USC1_SUSPENSION_COVER_B` is built from 3 free-form sheet bodies, and only solids were exported. Sheets now count too, if they have several faces or one free-form face. Single flat, cylindrical, conical, spherical or revolved sheets are construction aids and stay out (planes 1 m wide, a 2 m helper cylinder). 163 sheets in 40 parts are now exported. The cover now sits under the nose, x 1.10 to 1.95 m.
- **Current output:** `out\stl5`; `out\stl3` and `out\stl4` are earlier runs and the rebuilt `out\preview\faumax_tau_cad.blend` / `.glb` (210 MB). `out\stl` and `out\stl2` are the two faulty earlier runs (not deleted).
- **Still odd, but as placed in the CAD itself:** `E_DL01_FSG_Datalogger_2024` at the car origin, partly below the ground, and `E_DV03_DV_Switch` 0.48 m below the ground. Also the FS driver template head in the cockpit.
- **Positions checked:** loading a subassembly properly from `D:\FSLap_CAD\flat` (`make_flat.py`, one hard link per part name) gives exactly the positions of the tree, so the tree positions were never the problem.

## Linked copies and the owner's cleaned file (2026-11-12)

- **Linked copies:** parts carry linked copies (NX WAVE `LINKED_BODY`) of other parts' bodies for design reference. Exported, they overlapped the real parts: the louver carries the endplate, the main element bracket carries the main element, the undertray carries the radiator brackets, the tubing bracket carries the undertray sidewall. `export_stl.py` now leaves out a body that is only a linked copy, unless it is all the part has. A `_MIR` part mirrors the whole original, linked copy included, so the body with the same face count and size as the original's dropped copy goes too. 59 copies dropped. `s_fr01_consoles`, `s_sa00_steering_attachment` and `p_in00_cooling` are only linked bodies and stay as they are.
- **Current output:** `out\stl7`.
- **The owner's cleaned file:** `out\preview\faumax_tau_cad_clean.blend` (853 objects) is the owner's and is never written by the tools. `faumax_tau_cad_clean_fixed.blend` is built from it with `assemble_car.py ... out\keep_clean_fixed.json faumax_tau_cad_clean_fixed`. It contains the owner's objects plus 16 restored ones: rear wing endplates and their inserts, main element brackets, front hoop, left undertray sidewall, diffuser, undertray side wings, radiator duct, HV motor cables. That makes 869 objects and 3.0 million triangles, with no linked duplicates.
- `list_objects.py` / `compare_blends.py`: list a .blend's objects and compare two of them.

## Why the tree comes from the top part only

NX can't load the whole assembly from this copy, for two reasons:

- The saved component paths point to the laptop the model was last saved on.
- 117 part names exist twice (old versions in `alt*` folders), so a directory search is ambiguous (NX error 670024).

The top part still holds the full tree with absolute positions. The scripts therefore open each part on its own and place it themselves. The check renders show the arms meeting the hardpoints.

## Rules

- Never write to `D:\SVN`. Work only on the copy here.
- The FS Lap repository is public. CAD and anything exported from it stay out of git, here or in the git-ignored `high-end\data\`, until the owner decides what may be shown.
- The 2027 car (`D:\SVN\2027\...\0000_FAUMAX_UPSILON.prt`, 3 GB, same layout) has not been copied: the owner says its CAD will change a lot soon. The same scripts work on it.
