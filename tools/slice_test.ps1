# Acceptance run in the real game through the normal game flow: AI lap, frame times at Medium settings, photos of the car from all sides.
# Results in FSLapUE\Saved\SliceTest\<mode> (results.txt, PNGs). The editor must be closed.
#   tools\slice_test.ps1 [-Mode sim|arcade|free|city|keys]    keys presses every control and checks what it did
#   -Sound also records what the game plays, for tools\check_sound.py (you will hear it)
param([ValidateSet('sim', 'arcade', 'free', 'city', 'keys')][string]$Mode = 'sim', [string]$Engine = 'D:\UE\UE_5.8', [switch]$Sound)
$ErrorActionPreference = 'Stop'
$root = Split-Path -Parent $PSScriptRoot
$out = Join-Path $root "FSLapUE\Saved\SliceTest\$Mode"
New-Item -ItemType Directory -Force -Path $out | Out-Null
Remove-Item (Join-Path $out 'results.txt') -ErrorAction SilentlyContinue
$gameArgs = @("`"$(Join-Path $root 'FSLapUE\FSLapUE.uproject')`"", '-game', '-windowed', '-ResX=1920', '-ResY=1080', '-ForceRes', '-FSLapSliceTest', "-FSLapMode=$Mode", $(if ($Sound) { '-FSLapRecordAudio' } else { '-nosound' }), '-nosplash', '-unattended', "-abslog=`"$out\game.log`"")
$p = Start-Process -FilePath "$Engine\Engine\Binaries\Win64\UnrealEditor.exe" -ArgumentList $gameArgs -PassThru
if (-not $p.WaitForExit(1200000)) { $p.Kill(); throw "timed out; see $out\game.log" }
if (-not (Test-Path (Join-Path $out 'results.txt'))) { throw "no results.txt; see $out\game.log" }
Get-Content (Join-Path $out 'results.txt')
