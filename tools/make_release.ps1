# Zips a stand-alone build (section 2 of the README, "Make a stand-alone build") for the Releases page of the shared
# repository, with Submit runs.bat, so the team can drive without Unreal Engine:
#   powershell -File tools\make_release.ps1 -Build dist\FSLap_2026-11-27\Windows
# Writes dist\FSLap-<date>-Win64.zip. FSLapUE\Saved (laps, settings, the driver's name of this PC) is left out.
param([Parameter(Mandatory)][string]$Build, [string]$Date = (Get-Date -Format 'yyyy-MM-dd'))

$ErrorActionPreference = 'Stop'
$Root = Split-Path -Parent $PSScriptRoot
$Build = (Resolve-Path $Build).Path
if (-not (Test-Path (Join-Path $Build 'FSLapUE.exe'))) { throw "No FSLapUE.exe in $Build" }

Copy-Item -Force (Join-Path $Root 'Submit runs.bat') $Build
New-Item -ItemType Directory -Force (Join-Path $Build 'tools') | Out-Null
Copy-Item -Force (Join-Path $Root 'tools\submit_runs.ps1') (Join-Path $Build 'tools')

$Zip = Join-Path $Root "dist\FSLap-$Date-Win64.zip"
Remove-Item -Force -ErrorAction SilentlyContinue $Zip
# tar (Windows 10 and newer) writes zip files over 2 GB and keeps the folder layout; Compress-Archive in PowerShell 5.1 does not
& tar.exe -a -c -f $Zip --exclude 'FSLapUE/Saved' -C $Build .
if ($LASTEXITCODE) { throw 'tar failed' }
$names = & tar.exe -t -f $Zip
if ($names -match 'FSLapUE/Saved') { throw 'the zip contains FSLapUE/Saved' }
foreach ($need in 'FSLapUE.exe', 'Submit runs.bat', 'tools/submit_runs.ps1')
{
	if (-not ($names -match [regex]::Escape($need) + '$')) { throw "the zip has no $need" }
}
'{0}  {1:N2} GB' -f $Zip, ((Get-Item $Zip).Length / 1GB)
