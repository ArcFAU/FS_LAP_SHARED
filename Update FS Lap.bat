@echo off
rem Updates FS Lap from the shared repository and rebuilds it; your laps and settings stay. See tools\update.ps1.
powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0tools\update.ps1" %*
pause
