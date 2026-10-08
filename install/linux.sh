#!/usr/bin/env bash
# c-cellar — installer for Linux (Debian/Ubuntu and derivatives, Fedora/RHEL and derivatives, Arch and derivatives, openSUSE).
#
# It installs what is missing (a container engine, git, curl), gets the project if you do not have it yet, and starts the lab.
# Nothing is changed that is already fine, and it asks before it installs anything.
#
#   ./install/linux.sh                  from a copy of the project
#   curl -fsSL https://raw.githubusercontent.com/suto-sec/c-cellar/main/install/linux.sh | bash      without a copy
#
# Options (after `bash -s --` when piped):
#   -y, --yes        do not ask questions
#   --docker         use Docker instead of podman
#   --no-start       install only, do not start the lab
#   --dir DIR        where to put the project when it is downloaded (default: ~/c-cellar)
#   --dry-run        show what would be done, change nothing
#   -h, --help       this text
set -euo pipefail

REPO=https://github.com/suto-sec/c-cellar.git
YES=0 USE_DOCKER=0 START=1 DRY=0 DEST=$HOME/c-cellar

while (( $# )); do
  case $1 in
    -y|--yes) YES=1 ;;
    --docker) USE_DOCKER=1 ;;
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
ask()  { # question -> 0 for yes (the answer is read from the terminal even when the script itself is piped)
  (( YES )) && return 0
  local a=y
  if [[ -r /dev/tty ]]; then read -r -p "$1 [Y/n] " a </dev/tty || a=y; fi
  [[ ${a:-y} =~ ^[Yy]|^$ ]]
}

[[ $(uname -s) == Linux ]] || die "This installer is for Linux. On macOS use install/macos.sh, on Windows install/windows.ps1."
[[ $EUID -ne 0 ]] || warn "You are running as root: the lab is meant for a normal user. Continuing anyway."

# ---------------------------------------------------------------- what is this system
OSR=${OS_RELEASE:-/etc/os-release}            # (OS_RELEASE lets the tests pretend to be another distribution)
ID_LIKE_ALL=""
if [[ -r $OSR ]]; then ID_LIKE_ALL=$(. "$OSR"; echo "${ID:-} ${ID_LIKE:-}"); fi
PM=""
case " $ID_LIKE_ALL " in
  *" debian "*|*" ubuntu "*) PM=apt ;;
  *" fedora "*|*" rhel "*|*" centos "*) PM=dnf ;;
  *" arch "*) PM=pacman ;;
  *" suse "*|*" opensuse "*|*" sles "*) PM=zypper ;;
esac
if [[ -z $PM ]]; then                         # unknown distribution: go by the package manager that exists
  for c in apt-get dnf pacman zypper; do command -v "$c" >/dev/null && { PM=${c/apt-get/apt}; break; }; done
fi
[[ -n $PM ]] || die "I do not know this distribution. Install podman (or docker), git and curl yourself, then run: git clone $REPO && cd c-cellar && ./lab web"
say "System: ${ID_LIKE_ALL:-unknown} (package manager: $PM)"

SUDO=""
if [[ $EUID -ne 0 ]]; then
  command -v sudo >/dev/null || die "I need sudo to install packages. Install it, or install podman/docker, git and curl by hand."
  SUDO=sudo
fi

pkg_install() { # packages...
  case $PM in
    apt)    run $SUDO apt-get update -y; run $SUDO env DEBIAN_FRONTEND=noninteractive apt-get install -y "$@" ;;
    dnf)    run $SUDO dnf install -y "$@" ;;
    pacman) run $SUDO pacman -S --needed --noconfirm "$@" ;;
    zypper) run $SUDO zypper --non-interactive install "$@" ;;
  esac
}

# ---------------------------------------------------------------- git and curl
need=()
command -v git  >/dev/null || need+=(git)
command -v curl >/dev/null || need+=(curl)

# ---------------------------------------------------------------- the container engine
podman_ok() { command -v podman >/dev/null && podman --version | awk '{split($3,v,"."); exit !(v[1] > 4 || (v[1] == 4 && v[2] >= 3))}'; }
docker_ok() { command -v docker >/dev/null && docker info >/dev/null 2>&1; }

engine=""
if (( USE_DOCKER )); then
  docker_ok && engine=docker
else
  podman_ok && engine=podman
  [[ -z $engine ]] && docker_ok && { say "podman is missing or too old, but Docker works: using Docker"; engine=docker; }
fi

if [[ -z $engine ]]; then
  if (( USE_DOCKER )); then
    case $PM in
      apt)    need+=(docker.io) ;;
      dnf)    need+=(moby-engine) ;;
      pacman) need+=(docker) ;;
      zypper) need+=(docker) ;;
    esac
    engine=docker-new
  else
    case $PM in
      apt)    need+=(podman uidmap slirp4netns) ;;
      dnf)    need+=(podman) ;;
      pacman) need+=(podman slirp4netns fuse-overlayfs) ;;
      zypper) need+=(podman) ;;
    esac
    engine=podman-new
  fi
fi

if (( ${#need[@]} )); then
  say "To install: ${need[*]}"
  ask "Install these packages now (needs your password for sudo)?" || die "Cancelled. Nothing was installed."
  pkg_install "${need[@]}"
fi

if [[ $engine == podman-new ]] && ! (( DRY )); then
  podman_ok || die "The podman that was installed is older than 4.3 (the lab needs 4.3 or newer). Run this installer again with --docker."
  engine=podman
fi

# ---------------------------------------------------------------- rootless podman needs a range of sub-ids
if [[ $engine == podman* ]]; then
  me=${USER:-$(id -un)}
  if [[ $EUID -ne 0 ]] && ! grep -q "^$me:" /etc/subuid 2>/dev/null; then
    say "Giving $me a range of user ids for rootless podman"
    run $SUDO usermod --add-subuids 100000-165535 --add-subgids 100000-165535 "$me"
    (( DRY )) || podman system migrate >/dev/null 2>&1 || true
  fi
fi

# ---------------------------------------------------------------- Docker: the daemon and the docker group
if [[ $engine == docker-new ]]; then
  run $SUDO systemctl enable --now docker || warn "Could not start the Docker service by itself: start it, then run this installer again."
  if ! id -nG | tr ' ' '\n' | grep -qx docker && [[ $EUID -ne 0 ]]; then
    run $SUDO usermod -aG docker "${USER:-$(id -un)}"
    warn "You were added to the docker group. LOG OUT AND IN AGAIN (or reboot), then run this installer again."
    exit 0
  fi
  engine=docker
fi

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
