# Brings your copy of FS Lap up to date with the shared repository and rebuilds it.
#   Update FS Lap.bat
# Your laps (FSLapUE\Saved\Runs), settings, keys and wheel calibration (FSLapUE\Saved\SaveGames) are not in git, so
# the update never touches them. Changes you made to tracked files are put aside and put back (git pull --autostash).
# Never delete the folder and clone it again to update: that deletes FSLapUE\Saved with your laps.
$ErrorActionPreference = 'Stop'
$Root = Split-Path -Parent $PSScriptRoot
$Ue = if ($env:UE_ROOT) { $env:UE_ROOT } else { 'D:\UE\UE_5.8' }   # as in Play FS Lap.bat

$running = Get-CimInstance Win32_Process -Filter "Name='UnrealEditor.exe'" | Where-Object { $_.CommandLine -like '*FSLapUE*' }
if ($running) { Write-Host 'Close FS Lap first: the build cannot replace the files of a running game.'; exit 1 }
if (-not (Test-Path "$Ue\Engine\Build\BatchFiles\Build.bat")) { Write-Host "No Unreal Engine at ${Ue}: set UE_ROOT to your engine folder."; exit 1 }

$before = (& git.exe -C $Root rev-parse HEAD)
& git.exe -C $Root pull --ff-only --autostash origin main
if ($LASTEXITCODE)
{
	Write-Host ''
	Write-Host 'git could not update this copy (see above). Nothing was changed; your laps are safe.'
	exit 1
}
$after = (& git.exe -C $Root rev-parse HEAD)
if ($before -eq $after) { Write-Host 'Already up to date.' }
else
{
	Write-Host 'New in this version:'
	& git.exe -C $Root log --format='  %s' "$before..$after"
}

Write-Host 'Building (a few minutes after a big update) ...'
& "$Ue\Engine\Build\BatchFiles\Build.bat" FSLapUEEditor Win64 Development "-Project=$Root\FSLapUE\FSLapUE.uproject" -WaitMutex
if ($LASTEXITCODE) { Write-Host 'The build failed (see above). Send the owner the last lines.'; exit 1 }
$laps = @(Get-ChildItem "$Root\FSLapUE\Saved\Runs" -Recurse -Filter 'lap_*.csv' -ErrorAction SilentlyContinue).Count
Write-Host "Done. $(Get-Content "$Root\VERSION.txt" -ErrorAction SilentlyContinue) - your $laps saved lap(s) are still in FSLapUE\Saved\Runs."
