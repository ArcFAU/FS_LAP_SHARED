# FS Lap run data

This branch collects the laps that drivers send from FS Lap with `Submit runs.bat` (see the README on `main`, section "Collecting laps").

- `<driver>/<session>/lap_NN.csv.gz`: every simulation step (600 Hz) of a completed lap: inputs and the car's state.
- `<driver>/<session>/lap_NN.run.json.gz`: the same lap at 100 Hz in the LTS run format (`homo-lts.run.v1`), with the setup, the result, the vehicle model (`calculationKernel`) and the FS Lap version (`fslapVersion`).

The session folder name is the start time and the mode, for example `2026-11-15_041724_hotlap`.

The laps are stored in Git LFS. To get all of them:

```powershell
git clone --branch runs --single-branch https://github.com/ArcFAU/FS_LAP_SHARED.git FS_LAP_RUNS
cd FS_LAP_RUNS
git pull    # later: the new laps
```

Reading a lap: Python `pandas.read_csv("lap_01.csv.gz")` reads it as it is; in MATLAB, `gunzip` it first.

This repository is public: everything here can be seen by anyone.
