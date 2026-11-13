# Runs one of the Editor Python tools in a windowless full editor and checks its success marker.
#   tools\run_editor_script.ps1 import_ue.py FSLAP_IMPORT_OK
param([Parameter(Mandatory)][string]$Script, [Parameter(Mandatory)][string]$Marker, [string]$Engine = 'D:\UE\UE_5.8')
$ErrorActionPreference = 'Stop'
$root = Split-Path -Parent $PSScriptRoot
$log = Join-Path $root "data\$([IO.Path]::GetFileNameWithoutExtension($Script)).log"
$path = (Join-Path $PSScriptRoot $Script).Replace('\', '/')
$a = @("`"$(Join-Path $root 'FSLapUE\FSLapUE.uproject')`"", "-ExecCmds=`"py import runpy; runpy.run_path('$path')`"", '-unattended', '-nop4', '-nosplash', '-nullrhi', '-nosound', "-abslog=`"$log`"")
$p = Start-Process -FilePath "$Engine\Engine\Binaries\Win64\UnrealEditor-Cmd.exe" -ArgumentList $a -PassThru -WindowStyle Hidden
if (-not $p.WaitForExit(2400000)) { $p.Kill(); throw "timed out; see $log" }
if (-not (Select-String -Path $log -SimpleMatch $Marker -Quiet)) { Get-Content $log -Tail 25; throw "$Script did not finish; see $log" }
"$Marker found"
