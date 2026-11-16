@echo off
rem The SIL bridge for FS Lap's SIL mode (fslap_sil.py). No argument: the stand-in VCU (no firmware, tests the loop).
rem With the path of vcu_full.dll: the car's VCU firmware through the team's SIL (vehicle-controls\03_SIL).
if "%~1"=="" (python "%~dp0fslap_sil.py" --stand-in) else (python "%~dp0fslap_sil.py" --dll "%~1")
pause
