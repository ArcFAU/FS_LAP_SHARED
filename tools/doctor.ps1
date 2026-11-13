# Read-only environment check for FS Lap contributors.
# It makes no project, engine, registry, package, or Git changes.
param(
    [string]$Engine = 'D:\UE\UE_5.8',
    [switch]$Strict
)

$ErrorActionPreference = 'Stop'
$root = Split-Path -Parent $PSScriptRoot
$project = Join-Path $root 'FSLapUE\FSLapUE.uproject'
$requiredPython = @('numpy', 'PIL', 'scipy')
$problems = [System.Collections.Generic.List[string]]::new()

function Report([string]$Name, [bool]$Present, [string]$Detail) {
    $state = if ($Present) { 'OK  ' } else { 'MISS' }
    Write-Host "$state  $Name - $Detail"
    if (-not $Present) { $problems.Add("$Name: $Detail") }
}

Write-Host "FS Lap preflight: $root`n"
Report 'Repository' (Test-Path -LiteralPath (Join-Path $root '.git')) $root
Report 'Unreal project' (Test-Path -LiteralPath $project) $project
Report 'Unreal editor' (Test-Path -LiteralPath (Join-Path $Engine 'Engine\Binaries\Win64\UnrealEditor.exe')) $Engine
Report 'Validation runner' (Test-Path -LiteralPath (Join-Path $PSScriptRoot 'validate.ps1')) 'tools\validate.ps1'

$git = Get-Command git -ErrorAction SilentlyContinue
Report 'Git' ($null -ne $git) ($(if ($git) { $git.Source } else { 'install Git' }))
$lfs = Get-Command git-lfs -ErrorAction SilentlyContinue
Report 'Git LFS' ($null -ne $lfs) ($(if ($lfs) { $lfs.Source } else { 'install Git LFS' }))

$python = Get-Command py -ErrorAction SilentlyContinue
Report 'Python launcher' ($null -ne $python) ($(if ($python) { $python.Source } else { 'install Python with py launcher' }))
if ($python) {
    foreach ($module in $requiredPython) {
        & $python.Source -c "import $module" 2>$null
        Report "Python module $module" ($LASTEXITCODE -eq 0) 'required by standalone tools'
    }
}
else {
    foreach ($module in $requiredPython) {
        Report "Python module $module" $false 'Python launcher is unavailable'
    }
}

foreach ($path in @('E:\UE\livery', 'E:\HOT LAP\source', 'D:\VD')) {
    $exists = Test-Path -LiteralPath $path
    $state = if ($exists) { 'available read-only source' } else { 'optional external source unavailable' }
    Write-Host "INFO $path - $state"
}

if ($git) {
    $status = & $git.Source -C $root status --short 2>$null
    $count = @($status).Count
    Write-Host "INFO Git worktree - $count changed/untracked entries"
}

if ($problems.Count -eq 0) {
    Write-Host "`nPreflight passed."
    exit 0
}

Write-Host "`nPreflight found $($problems.Count) missing prerequisite(s):"
$problems | ForEach-Object { Write-Host "- $_" }
if ($Strict) { exit 1 }
