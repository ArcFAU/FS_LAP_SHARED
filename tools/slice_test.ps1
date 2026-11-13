# Acceptance run in the real game through the normal game flow: AI lap, frame times at Medium settings, photos of the car from all sides.
# Results in FSLapUE\Saved\SliceTest\<mode> (results.txt, PNGs). The editor must be closed.
#   tools\slice_test.ps1 [-Mode sim|arcade|free|city|keys|lts|adtm]    keys presses every control and checks what it did;
#   lts first has the real LTS (MATLAB) compute the reference for the stock setup, then runs the hot lap against it;
#   adtm runs the hot lap with the team's ADTM as the vehicle model (code from tools\adtm\adtm_codegen.m)
#   -Sound also records what the game plays, for tools\check_sound.py (you will hear it); -Adtm runs any mode with the ADTM
param([ValidateSet('sim', 'arcade', 'free', 'city', 'keys', 'lts', 'adtm')][string]$Mode = 'sim', [string]$Engine = 'D:\UE\UE_5.8', [switch]$Sound, [switch]$Adtm)
$ErrorActionPreference = 'Stop'
$root = Split-Path -Parent $PSScriptRoot
$out = Join-Path $root "FSLapUE\Saved\SliceTest\$Mode"
New-Item -ItemType Directory -Force -Path $out | Out-Null
Remove-Item (Join-Path $out 'results.txt') -ErrorAction SilentlyContinue
$gameArgs = @("`"$(Join-Path $root 'FSLapUE\FSLapUE.uproject')`"", '-game', '-windowed', '-ResX=1920', '-ResY=1080', '-ForceRes', '-FSLapSliceTest', "-FSLapMode=$Mode", $(if ($Sound) { '-FSLapRecordAudio' } else { '-nosound' }), '-nosplash', '-unattended', "-abslog=`"$out\game.log`"")
if ($Adtm) { $gameArgs += '-FSLapAdtm' }
$p = Start-Process -FilePath "$Engine\Engine\Binaries\Win64\UnrealEditor.exe" -ArgumentList $gameArgs -PassThru
if (-not $p.WaitForExit(1200000)) { $p.Kill(); throw "timed out; see $out\game.log" }
if (-not (Test-Path (Join-Path $out 'results.txt'))) { throw "no results.txt; see $out\game.log" }
Get-Content (Join-Path $out 'results.txt')
