#!/bin/bash
# Downloads and assembles everything the machine is made of into site/work/ (what a CI job runs; locally it skips what is already there).
# All versions are pinned; the downloads are checked against their checksums where the source gives a stable file.
#   work/v86src             the v86 repository at one commit (its BIOS files and the two tools that pack a filesystem)
#   work/v86pkg/package     the v86 npm package (libv86.js, libv86.mjs, v86.wasm)
#   work/kernel/buildroot-bzimage.bin   a kernel with 9p and virtio, built by the author of v86
#   work/alpine/root        a 32-bit Alpine root filesystem with gcc, bash, coreutils, make, binutils, musl-dev, procps
set -euo pipefail
cd "$(dirname "$0")"
W=work; mkdir -p "$W"
V86_COMMIT=cc37f35c43bf14865d74ad76879ecf9410f69eb1
V86_NPM=0.5.472
KERNEL_URL=https://i.copy.sh/buildroot-bzimage.bin
KERNEL_SHA=7befbaea31e249d9a518c4b95fa42b2a193d0e3de46250d617cbdeb866ee28b0
ALPINE=https://dl-cdn.alpinelinux.org/alpine/v3.21
# Alpine keeps only the newest file of a package in its repository, so these two cannot be pinned by name (a pinned apk-tools-static broke the build within
# two weeks): the current names are read from the mirror at build time over HTTPS. The minirootfs is checked against the sha256 Alpine publishes with it.
PACKAGES="bash coreutils grep sed findutils make binutils gcc musl-dev procps"

check() { echo "$2  $1" | sha256sum -c --quiet - || { echo "checksum mismatch: $1" >&2; exit 1; }; }

if [ ! -d "$W/v86src/bios" ]; then
  rm -rf "$W/v86src"; mkdir "$W/v86src"
  ( cd "$W/v86src" && git init -q && git fetch -q --depth 1 https://github.com/copy/v86 "$V86_COMMIT" && git checkout -q FETCH_HEAD )
fi
if [ ! -f "$W/v86pkg/package/build/v86.wasm" ]; then
  rm -rf "$W/v86pkg"; mkdir -p "$W/v86pkg"
  curl -fsSL "https://registry.npmjs.org/v86/-/v86-$V86_NPM.tgz" | tar -xz -C "$W/v86pkg"
fi
if [ ! -f "$W/kernel/buildroot-bzimage.bin" ]; then
  mkdir -p "$W/kernel"; curl -fsSL -o "$W/kernel/buildroot-bzimage.bin" "$KERNEL_URL"
fi
check "$W/kernel/buildroot-bzimage.bin" "$KERNEL_SHA"

if [ ! -x "$W/alpine/root/usr/bin/gcc" ]; then
  rm -rf "$W/alpine"; mkdir -p "$W/alpine/tools" "$W/alpine/root"
  RELEASES=$(curl -fsSL "$ALPINE/releases/x86/latest-releases.yaml" | grep -A8 "flavor: alpine-minirootfs")
  MINIROOTFS=$(echo "$RELEASES" | sed -n 's/^ *file: *//p' | head -n 1)
  MINIROOTFS_SHA=$(echo "$RELEASES" | sed -n 's/^ *sha256: *//p' | head -n 1)
  APKSTATIC=$(curl -fsSL "$ALPINE/main/x86_64/" | grep -o 'apk-tools-static-[0-9][0-9a-z._-]*\.apk' | sort -u | tail -n 1)
  [ -n "$MINIROOTFS" ] && [ -n "$MINIROOTFS_SHA" ] && [ -n "$APKSTATIC" ] || { echo "could not find the current Alpine files at $ALPINE" >&2; exit 1; }
  echo "Alpine: $MINIROOTFS, $APKSTATIC"
  curl -fsSL -o "$W/alpine/$MINIROOTFS" "$ALPINE/releases/x86/$MINIROOTFS"
  curl -fsSL -o "$W/alpine/$APKSTATIC" "$ALPINE/main/x86_64/$APKSTATIC"
  check "$W/alpine/$MINIROOTFS" "$MINIROOTFS_SHA"
  tar -xzf "$W/alpine/$APKSTATIC" -C "$W/alpine/tools"
  tar -xzf "$W/alpine/$MINIROOTFS" -C "$W/alpine/root"
  # no root needed: user mode, and the packages' install scripts are skipped (the root filesystem only has to exist, not to boot by itself)
  # (apk reports "errors updating directory permissions" when it is not root: harmless here)
  "$W/alpine/tools/sbin/apk.static" --usermode --root "$PWD/$W/alpine/root" --arch x86 --allow-untrusted --no-scripts --no-cache \
    -X "$ALPINE/main" -X "$ALPINE/community" -U add $PACKAGES || true
  [ -x "$W/alpine/root/usr/bin/gcc" ] || { echo "the Alpine packages were not installed" >&2; exit 1; }
fi
echo "parts ready in $W/"
