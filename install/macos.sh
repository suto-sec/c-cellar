#!/usr/bin/env bash
# c-cellar — installer for macOS (Intel and Apple silicon).
#
# It makes sure git and Docker Desktop are there (installing them with Homebrew when needed), waits for Docker to be ready, gets the project
# if you do not have it yet, and starts the lab. It asks before it installs anything.
#
#   ./install/macos.sh                  from a copy of the project
#   curl -fsSL https://raw.githubusercontent.com/suto-sec/c-cellar/main/install/macos.sh | bash      without a copy
#
# Options (after `bash -s --` when piped):
#   -y, --yes        do not ask questions
#   --no-start       install only, do not start the lab
#   --dir DIR        where to put the project when it is downloaded (default: ~/c-cellar)
#   --dry-run        show what would be done, change nothing
#   -h, --help       this text
set -euo pipefail

REPO=https://github.com/suto-sec/c-cellar.git
YES=0 START=1 DRY=0 DEST=$HOME/c-cellar

while (( $# )); do
  case $1 in
    -y|--yes) YES=1 ;;
    --no-start) START=0 ;;
    --dry-run) DRY=1 ;;
    --dir) DEST=${2:?--dir needs a folder}; shift ;;
    -h|--help) sed -n '2,/^set -e/p' "$0" 2>/dev/null | sed '$d;s/^# \{0,1\}//' ; exit 0 ;;
    *) echo "Unknown option: $1 (try --help)" >&2; exit 2 ;;
  esac
  shift
done

say()  { printf '\033[1m==> %s\033[0m\n' "$*"; }
warn() { printf '\033[33m!! %s\033[0m\n' "$*" >&2; }
die()  { printf '\033[31mxx %s\033[0m\n' "$*" >&2; exit 1; }
run()  { if (( DRY )); then printf '   [dry run] %s\n' "$*"; else "$@"; fi; }
ask()  {
  (( YES )) && return 0
  local a=y
  if [[ -r /dev/tty ]]; then read -r -p "$1 [Y/n] " a </dev/tty || a=y; fi
  [[ ${a:-y} =~ ^[Yy]|^$ ]]
}

[[ $(uname -s) == Darwin ]] || die "This installer is for macOS. On Linux use install/linux.sh, on Windows install/windows.ps1."
say "macOS $(sw_vers -productVersion 2>/dev/null || echo '?') on $(uname -m)"

# ---------------------------------------------------------------- git (comes with Apple's command line tools)
if ! command -v git >/dev/null || ! git --version >/dev/null 2>&1; then
  say "git is missing: asking macOS to install the command line tools"
  run xcode-select --install || true
  (( DRY )) || die "A window should have opened: finish installing the command line tools there, then run this installer again."
fi

# ---------------------------------------------------------------- Docker Desktop
docker_ready() { command -v docker >/dev/null && docker info >/dev/null 2>&1; }
if ! docker_ready; then
  if [[ ! -d /Applications/Docker.app ]]; then
    if ! command -v brew >/dev/null; then
      warn "Homebrew (the macOS package installer) is not installed."
      if ask "Install Homebrew now? (the official installer from brew.sh; it asks for your password)"; then
        install_brew() { /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"; }
        run install_brew            # (the official Homebrew installer from brew.sh)
        # (Apple silicon puts it in /opt/homebrew, Intel in /usr/local)
        for b in /opt/homebrew/bin/brew /usr/local/bin/brew; do [[ -x $b ]] && eval "$($b shellenv)" && break; done
      else
        die "Without Homebrew, install Docker Desktop by hand from https://www.docker.com/products/docker-desktop/ , start it, then run this installer again."
      fi
    fi
    say "Installing Docker Desktop with Homebrew (a large download)"
    ask "Install Docker Desktop now?" || die "Cancelled. Nothing was installed."
    run brew install --cask docker
  fi
  say "Starting Docker Desktop"
  run open -a Docker
  if (( ! DRY )); then
    echo "   The first time, Docker Desktop asks you to accept its terms and to allow its helper (your password): do that in its window."
    for i in $(seq 1 120); do
      docker_ready && break
      (( i % 10 == 0 )) && echo "   ... waiting for Docker to be ready ($((i * 3))s)"
      sleep 3
    done
    docker_ready || die "Docker is not ready after 6 minutes. Open Docker Desktop, wait for the whale icon to stop moving, then run this installer again."
  fi
fi
say "Docker is ready"

# ---------------------------------------------------------------- the project
HERE=""
if [[ -n ${BASH_SOURCE[0]:-} && -f ${BASH_SOURCE[0]} ]]; then
  cand=$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)
  [[ -x $cand/lab ]] && HERE=$cand
fi
if [[ -z $HERE ]]; then
  if [[ -x $DEST/lab ]]; then HERE=$DEST; say "Project found in $DEST"
  else
    say "Downloading the project into $DEST"
    run git clone --depth 1 "$REPO" "$DEST"
    HERE=$DEST
  fi
fi
say "Project: $HERE"

if (( START )); then
  if (( DRY )); then say "[dry run] would run: cd $HERE && ./lab web"; exit 0; fi
  say "Starting the lab. The first time it builds the image (5 to 15 minutes, about 2 GB); later starts take seconds."
  cd "$HERE" && exec ./lab web
fi
say "Done. Start the lab with:  cd $HERE && ./lab web"
