<#
.SYNOPSIS
  c-cellar - installer for Windows 10 (2004 or newer) and Windows 11.

.DESCRIPTION
  The lab runs in Linux containers, so on Windows it lives inside WSL 2 (the Windows Subsystem for Linux). This script:
    1. installs WSL if it is missing (this needs a restart; run the script again afterwards), then sets up Ubuntu 24.04
       (you invent a Linux user name and password once),
    2. checks that Ubuntu has a normal user,
    3. installs podman, git and curl inside Ubuntu (or uses Docker Desktop with -Docker),
    4. downloads the project into your Ubuntu home folder and starts the lab.
  Run it in PowerShell (Start > "Windows PowerShell"). The first step needs an administrator PowerShell
  (right-click > "Run as administrator"); the others do not.

  Run it again whenever it stops: it skips what is already done.

.EXAMPLE
  powershell -ExecutionPolicy Bypass -File install\windows.ps1

.EXAMPLE
  & ([scriptblock]::Create((irm https://raw.githubusercontent.com/suto-sec/c-cellar/main/install/windows.ps1)))

.PARAMETER Docker
  Use Docker Desktop (installed with winget if missing) instead of podman inside Ubuntu.
.PARAMETER NoStart
  Install only, do not start the lab.
.PARAMETER Distro
  The WSL distribution to use (default Ubuntu-24.04).
.PARAMETER DryRun
  Show what would be done, change nothing.
#>
param(
  [switch]$Docker,
  [switch]$NoStart,
  [switch]$DryRun,
  [string]$Distro = 'Ubuntu-24.04'
)

$ErrorActionPreference = 'Stop'
$Raw = 'https://raw.githubusercontent.com/suto-sec/c-cellar/main/install/linux.sh'

function Say($m)  { Write-Host "==> $m" -ForegroundColor Cyan }
function Warn($m) { Write-Host "!! $m" -ForegroundColor Yellow }
function Die($m)  { Write-Host "xx $m" -ForegroundColor Red; exit 1 }
function Do-It($what, [scriptblock]$block) {
  if ($DryRun) { Write-Host "   [dry run] $what" } else { & $block }
}
function Test-Admin {
  $p = New-Object Security.Principal.WindowsPrincipal([Security.Principal.WindowsIdentity]::GetCurrent())
  return $p.IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)
}
# the names of the installed WSL distributions (wsl.exe prints UTF-16, which shows up as NUL characters)
function Get-Distros {
  try { $out = & wsl.exe -l -q 2>$null } catch { return @() }
  if ($LASTEXITCODE -ne 0 -or -not $out) { return @() }
  return @($out | ForEach-Object { ($_ -replace "`0", '').Trim() } | Where-Object { $_ })
}

if ($env:OS -ne 'Windows_NT') { Die 'This installer is for Windows. On Linux use install/linux.sh, on macOS install/macos.sh.' }
$build = [Environment]::OSVersion.Version.Build
if ($build -lt 19041) { Die "Your Windows build ($build) is too old for WSL 2. Update Windows (Windows 10 version 2004 or newer is needed)." }
Say "Windows build $build"

# ---------------------------------------------------------------- 1. WSL and Ubuntu
$wslOk = $false
if (Get-Command wsl.exe -ErrorAction SilentlyContinue) {
  try { & wsl.exe --status *> $null; $wslOk = ($LASTEXITCODE -eq 0) } catch { $wslOk = $false }
}
if (-not $wslOk) {
  if (-not (Test-Admin)) {
    Warn 'Installing WSL needs an administrator PowerShell.'
    if ($PSCommandPath) {
      Say 'Opening an administrator PowerShell (accept the Windows prompt)...'
      $a = "-NoExit -ExecutionPolicy Bypass -File `"$PSCommandPath`" -Distro $Distro"
      if ($Docker) { $a += ' -Docker' }
      if ($NoStart) { $a += ' -NoStart' }
      if ($DryRun) { $a += ' -DryRun' }
      if (-not $DryRun) { Start-Process powershell.exe -Verb RunAs -ArgumentList $a }
      exit 0
    }
    Die 'Close this window, open PowerShell with "Run as administrator" and run the same command again.'
  }
  Say "Installing WSL (a download of a few hundred MB)"
  Do-It "wsl --install -d $Distro --no-launch" { & wsl.exe --install -d $Distro --no-launch }
  Write-Host ''
  Say 'WSL is installed. RESTART THE COMPUTER NOW (Windows needs it), then run this installer again.'
  exit 0
}
Say 'WSL is installed'

if ((Get-Distros) -notcontains $Distro) {
  Write-Host ''
  Say "Setting up $Distro. It will ask you to invent a Linux user name and a password (the password is not shown while you type: that is normal)."
  Write-Host "     When you see a prompt like  name@PC:~$  type  exit  and press Enter to come back here."
  Do-It "wsl --install -d $Distro" { & wsl.exe --install -d $Distro }
  if (-not $DryRun -and (Get-Distros) -notcontains $Distro) {
    Die "$Distro is not set up yet. Open '$Distro' from the Start menu, create the user, then run this installer again."
  }
}
Say "$Distro is installed"

# ---------------------------------------------------------------- 2. does Ubuntu have a normal user?
$uid = ''
try { $uid = (& wsl.exe -d $Distro -- id -u 2>$null | Out-String).Trim() } catch { }
if ($uid -eq '0' -or $uid -eq '') {
  Write-Host ''
  Say "Open '$Distro' from the Start menu once. It asks you to create a Linux user name and password"
  Write-Host '     (the password is not shown while you type: that is normal). Then run this installer again.'
  exit 0
}
Say "Linux user ready (id $uid)"

# ---------------------------------------------------------------- 3. the container engine
if ($Docker) {
  $dockerOk = $false
  try { & docker.exe version *> $null; $dockerOk = ($LASTEXITCODE -eq 0) } catch { $dockerOk = $false }
  if (-not $dockerOk) {
    if (-not (Get-Command winget -ErrorAction SilentlyContinue)) { Die 'winget is not available. Install Docker Desktop by hand from https://www.docker.com/products/docker-desktop/ , enable WSL integration for Ubuntu, and run this installer again.' }
    Say 'Installing Docker Desktop with winget'
    Do-It 'winget install -e --id Docker.DockerDesktop' { & winget.exe install -e --id Docker.DockerDesktop --accept-package-agreements --accept-source-agreements }
    Write-Host ''
    Say 'Start Docker Desktop, accept its terms, open Settings > Resources > WSL integration, switch on'
    Write-Host "     '$Distro', press Apply, then run this installer again."
    exit 0
  }
  Say 'Docker Desktop is available'
}

# ---------------------------------------------------------------- 4. packages, project and start (inside Ubuntu)
$linuxArgs = '-y'
if ($Docker) { $linuxArgs += ' --docker' }
$linuxArgs += ' --no-start'
Say 'Installing podman, git and curl inside Ubuntu (it asks for your Linux password) and downloading the project into ~/c-cellar'
Do-It "wsl -d $Distro -- bash -lc 'curl -fsSL $Raw | bash -s -- $linuxArgs'" {
  & wsl.exe -d $Distro -- bash -lc "curl -fsSL $Raw | bash -s -- $linuxArgs"
  if ($LASTEXITCODE -ne 0) { Die 'The Linux part failed (see above). Fix the problem and run this installer again.' }
}

if (-not $NoStart) {
  Say 'Starting the lab. The first time it builds the image (5 to 15 minutes, about 2 GB); later starts take seconds.'
  Do-It "wsl -d $Distro -- bash -lc 'cd ~/c-cellar && ./lab web'" {
    & wsl.exe -d $Distro -- bash -lc 'cd ~/c-cellar && ./lab web'
    if ($LASTEXITCODE -ne 0) { Die 'The lab did not start (see above).' }
  }
  Write-Host ''
  Say 'Done. Open  http://localhost:8080  in your browser.'
  Write-Host "     To start it again later:  wsl -d $Distro -- bash -lc 'cd ~/c-cellar && ./lab web'"
  Write-Host "     To stop it:               wsl -d $Distro -- bash -lc 'cd ~/c-cellar && ./lab reset'"
} else {
  Say "Done. Start it with:  wsl -d $Distro -- bash -lc 'cd ~/c-cellar && ./lab web'"
}
