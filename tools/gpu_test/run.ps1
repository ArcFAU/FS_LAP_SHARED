# Stage 0 GPU benchmark for FS Lap: builds the test scene if it is missing, then runs the game
# with -FSLapGpuBench. Results: FSLapUE\Saved\GpuBench (results.csv, one screenshot per case,
# nvidia_smi.csv with graphics-memory use during the run). The first run compiles shaders and takes a while.
param(
    [string]$Engine = 'D:\UE\UE_5.8',
    [switch]$MapOnly,
    [switch]$BenchOnly
)
$ErrorActionPreference = 'Stop'
$root = Split-Path -Parent (Split-Path -Parent $PSScriptRoot)
$project = Join-Path $root 'FSLapUE\FSLapUE.uproject'
$out = Join-Path $root 'FSLapUE\Saved\GpuBench'
$mapDone = Join-Path $out 'map_ok.txt'   # written only after the map step really succeeded
New-Item -ItemType Directory -Force -Path $out | Out-Null

if (-not $BenchOnly -and -not (Test-Path $mapDone)) {
    $script = (Join-Path $PSScriptRoot 'make_map.py').Replace('\', '/')
    $log = Join-Path $out 'make_map.log'
    Remove-Item (Join-Path $root 'FSLapUE/Content/Test/GpuTest.umap') -ErrorAction SilentlyContinue   # a map left by a failed run blocks a new one
    $mapArgs = @("`"$project`"", "-ExecCmds=`"py import runpy; runpy.run_path('$script')`"", '-unattended', '-nop4', '-nosplash', '-nullrhi', '-nosound', "-abslog=`"$log`"")
    $p = Start-Process -FilePath "$Engine\Engine\Binaries\Win64\UnrealEditor-Cmd.exe" -ArgumentList $mapArgs -PassThru -WindowStyle Hidden
    if (-not $p.WaitForExit(1500000)) { $p.Kill(); throw "The test map step timed out; see $log" }
    if (-not (Select-String -Path $log -SimpleMatch 'FSLAP_MAP_OK' -Quiet)) { throw "The test map was not created; see $log" }
    Get-Date | Set-Content -Path $mapDone
    "map created"
}
if ($MapOnly) { return }

$log = Join-Path $out 'bench.log'
$smi = Join-Path $out 'nvidia_smi.csv'
'timestamp, memory.used [MiB], utilization.gpu [%], clocks.gr [MHz], power.draw [W]' | Set-Content -Path $smi -Encoding ascii
$gameArgs = @("`"$project`"", '/Game/Test/GpuTest', '-game', '-windowed', '-ResX=1920', '-ResY=1080', '-ForceRes', '-FSLapGpuBench', '-nosound', '-nosplash', '-unattended', "-abslog=`"$log`"")
$p = Start-Process -FilePath "$Engine\Engine\Binaries\Win64\UnrealEditor.exe" -ArgumentList $gameArgs -PassThru
while (-not $p.HasExited) {
    nvidia-smi --query-gpu=timestamp,memory.used,utilization.gpu,clocks.gr,power.draw --format=csv,noheader,nounits | Add-Content -Path $smi -Encoding ascii
    Start-Sleep -Seconds 2
}
"exit code $($p.ExitCode)"
if (Test-Path (Join-Path $out 'results.csv')) { Get-Content (Join-Path $out 'results.csv') } else { throw "no results.csv; see $log" }
