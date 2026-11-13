# Reproducible FS Lap regression entry point.
#
# Default: deterministic physics only, no Unreal Editor required.
# -Game: also exercise the real-game acceptance flows.  Close Unreal Editor
#        before using this option.
param(
    [switch]$Game,
    [string]$Engine = 'D:\UE\UE_5.8'
)

$ErrorActionPreference = 'Stop'
$root = Split-Path -Parent $PSScriptRoot
$physics = Join-Path $PSScriptRoot 'physics_test\run.bat'
$physicsBinary = Join-Path $root 'data\physics_test\ailap.exe'

if (-not (Test-Path -LiteralPath $physics)) {
    throw "missing physics test runner: $physics"
}

# Compile once and run the default simulation/hot-lap mode.  The existing batch
# runner deliberately owns the compiler/toolchain setup.
Write-Host "`n=== physics: sim ==="
& $physics
if ($LASTEXITCODE -ne 0) {
    throw "physics validation failed for sim (exit code $LASTEXITCODE)"
}
if (-not (Test-Path -LiteralPath $physicsBinary)) {
    throw "physics runner succeeded but did not create $physicsBinary"
}

# Calling the built test executable avoids repeating the C++ compile for every
# other deterministic mode.  `ailap` uses no argument for sim; passing the
# word "sim" would instead be treated as a trace-file name.
$modes = @('sim', 'arcade', 'free', 'city')
foreach ($mode in $modes | Select-Object -Skip 1) {
    Write-Host "`n=== physics: $mode ==="
    & $physicsBinary $mode
    if ($LASTEXITCODE -ne 0) {
        throw "physics validation failed for $mode (exit code $LASTEXITCODE)"
    }
}

if (-not $Game) {
    Write-Host "`nPhysics validation passed for $($modes.Count) modes."
    exit 0
}

$slice = Join-Path $PSScriptRoot 'slice_test.ps1'
if (-not (Test-Path -LiteralPath $slice)) {
    throw "missing game test runner: $slice"
}

foreach ($mode in @('sim', 'city', 'keys')) {
    Write-Host "`n=== game: $mode ==="
    & $slice -Mode $mode -Engine $Engine
}

Write-Host "`nPhysics and game validation passed. Evidence is in FSLapUE\\Saved\\SliceTest."
