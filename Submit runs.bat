@echo off
rem Sends the laps you drove to the team (the branch "runs" of the shared repository). See tools\submit_runs.ps1.
powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0tools\submit_runs.ps1" %*
pause
