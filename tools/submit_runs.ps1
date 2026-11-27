# Sends the laps you drove to the team: every completed lap in FSLapUE\Saved\Runs that is not sent yet goes to the
# branch "runs" of the shared repository, as runs/<your name>/<session>/lap_NN.csv.gz and lap_NN.run.json.gz.
#   Submit runs.bat                       (asks your name the first time and remembers it in FSLapUE\Saved\driver.txt)
#   powershell -File tools\submit_runs.ps1 -Driver Johny
# Your laps stay where they are. A copy of what was sent is kept in FSLapUE\Saved\RunsUpload (a small clone of the
# "runs" branch without the other drivers' laps). If sending fails (no internet, no access), the laps are sent next time.
# The shared repository is public: your name and your laps can be seen by anyone.
param([string]$Driver, [string]$Remote)

$ErrorActionPreference = 'Stop'
$Root = Split-Path -Parent $PSScriptRoot
$Saved = Join-Path $Root 'FSLapUE\Saved'
$Runs = Join-Path $Saved 'Runs'
$Outbox = Join-Path $Saved 'RunsUpload'
$NameFile = Join-Path $Saved 'driver.txt'
$env:GIT_LFS_SKIP_SMUDGE = '1'   # never download the other drivers' laps

function Invoke-Git
{
	& git.exe @args
	if ($LASTEXITCODE) { throw "git $($args -join ' ') failed" }
}

function Compress-File($Source, $Target)
{
	$tmp = "$Target.tmp"   # renamed when complete, so a half-written file is never taken as sent
	$in = [IO.File]::OpenRead($Source)
	$out = [IO.File]::Create($tmp)
	$gz = New-Object IO.Compression.GZipStream($out, [IO.Compression.CompressionMode]::Compress)
	try { $in.CopyTo($gz) } finally { $gz.Dispose(); $out.Dispose(); $in.Dispose() }
	Move-Item -Force $tmp $Target
}

$null = & git.exe lfs version   # no 2> redirection: Windows PowerShell turns a native program's stderr into an error
if ($LASTEXITCODE) { throw 'Git LFS is missing: install it from https://git-lfs.com, then run "git lfs install" once.' }
if (-not (Test-Path $Runs)) { Write-Host 'No laps yet: drive a complete lap first.'; exit 0 }

if (-not $Driver -and (Test-Path $NameFile)) { $Driver = (Get-Content $NameFile -Raw).Trim() }
while (-not $Driver)
{
	$Driver = (Read-Host 'Your name (it names your folder in the shared repository, which anyone can see)').Trim()
}
$Folder = ($Driver -replace '[^A-Za-z0-9_-]+', '_').Trim('_')
if (-not $Folder) { throw "Use letters or digits in the name: '$Driver'" }
Set-Content -Encoding UTF8 $NameFile $Driver

if (-not (Test-Path (Join-Path $Outbox '.git')))
{
	Write-Host 'First submission: setting up FSLapUE\Saved\RunsUpload ...'
	if (-not $Remote) { $Remote = (& git.exe -C $Root remote get-url origin) }
	if (-not $Remote) { $Remote = 'https://github.com/ArcFAU/FS_LAP_SHARED.git' }   # the stand-alone build (zip) is no git clone
	Invoke-Git clone --quiet --branch runs --single-branch --depth 1 $Remote $Outbox
	Invoke-Git -C $Outbox lfs install --local   # the hook that uploads the laps on push
}
else
{
	& git.exe -C $Outbox pull --quiet --rebase   # offline is fine: the push below then fails and the laps go next time
}

# every lap whose two files are complete (the game writes the .run.json last) and not sent yet
$new = 0
foreach ($json in Get-ChildItem $Runs -Recurse -Filter 'lap_*.run.json')
{
	$csv = $json.FullName -replace '\.run\.json$', '.csv'
	if (-not (Test-Path $csv) -or $json.LastWriteTime -gt (Get-Date).AddSeconds(-10)) { continue }
	$target = Join-Path $Outbox (Join-Path $Folder $json.Directory.Name)
	$base = $json.Name -replace '\.run\.json$', ''
	if ((Test-Path "$target\$base.csv.gz") -and (Test-Path "$target\$base.run.json.gz")) { continue }
	New-Item -ItemType Directory -Force $target | Out-Null
	Compress-File $csv "$target\$base.csv.gz"
	Compress-File $json.FullName "$target\$base.run.json.gz"
	$new++
}

Invoke-Git -C $Outbox add -A
if (& git.exe -C $Outbox status --porcelain)
{
	# your name, not your git identity: no e-mail address goes into the public repository
	Invoke-Git -C $Outbox -c "user.name=$Driver" -c "user.email=$Folder@fslap.invalid" commit --quiet -m "Laps from ${Driver}: $new"
}
$ahead = & git.exe -C $Outbox rev-list --count '@{u}..HEAD'
if (-not $ahead -or [int]$ahead -eq 0) { Write-Host "Nothing new to send (laps sent before are in FSLapUE\Saved\RunsUpload\$Folder)."; exit 0 }

for ($try = 1; ; $try++)
{
	& git.exe -C $Outbox push --quiet origin runs
	if (-not $LASTEXITCODE) { break }
	if ($try -ge 3)
	{
		Write-Host ''
		Write-Host 'Could not send the laps. They are kept and go up the next time you run this.'
		Write-Host 'No internet, or your GitHub account has no write access to the shared repository: ask the owner to add you.'
		exit 1
	}
	& git.exe -C $Outbox pull --quiet --rebase   # another driver sent laps at the same moment
}
Write-Host "All your laps are sent ($new new this time, as $Driver). Thank you!"
