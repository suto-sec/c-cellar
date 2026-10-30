#!/bin/bash
# Makes site/dist/vm/ : the Linux machine of the browser edition. It does NOT download anything: it packs what is already in site/work/
#   work/v86pkg/package/build/{libv86.js,v86.wasm}   the emulator (npm package v86)
#   work/v86src/bios/{seabios.bin,vgabios.bin}       its BIOS images
#   work/kernel/buildroot-bzimage.bin                a kernel with 9p and virtio (from copy.sh, the author of v86)
#   work/alpine/root/                                the 32-bit Alpine root filesystem with gcc, bash, coreutils, make (made with apk.static)
# and uses the two tools of the v86 repository (fs2json.py, copy-to-sha256.py) to turn the root filesystem into the files v86 loads on demand.
set -euo pipefail
cd "$(dirname "$0")"
W=work; OUT=dist/vm
cp -a overlay/. "$W/alpine/root/"   # the boot script and lab-compile (tracked in overlay/, the rest of the root is downloaded)
chmod 755 "$W/alpine/root/etc/lab-boot.sh" "$W/alpine/root/usr/local/bin/lab-compile" "$W/alpine/root/usr/local/bin/lab-build"
rm -rf "$OUT" "$W/img"; mkdir -p "$OUT/rootfs" "$W/img/flat"   # (copy-to-sha256.py does not create its output folder)
# (member names must NOT start with "./": fs2json would make one top-level entry called "." and nothing would be at /mnt/etc...)
( cd "$W/alpine/root" && tar --numeric-owner --owner=0 --group=0 -cf "../../img/fs.tar" $(ls -A) )
# (the two tools print a line per file: keep that in a log, show its end only if one of them fails)
quiet() { "$@" > "$W/img/pack.log" 2>&1 || { tail -n 15 "$W/img/pack.log"; echo "build_vm: $1 $2 failed" >&2; exit 1; }; }
quiet python3 "$W/v86src/tools/fs2json.py" --out "$W/img/fs.json" "$W/img/fs.tar"
quiet python3 "$W/v86src/tools/copy-to-sha256.py" "$W/img/fs.tar" "$W/img/flat"
cp "$W/img/fs.json" "$OUT/rootfs/fs.json"
cp -r "$W/img/flat" "$OUT/rootfs/flat"
python3 - "$OUT/rootfs/fs.json" <<'PY'
import json, sys
top = [e[0] for e in json.load(open(sys.argv[1]))["fsroot"]]
assert "etc" in top and "bin" in top and "lab-boot.sh" in [e[0] for e in next(e for e in json.load(open(sys.argv[1]))["fsroot"] if e[0] == "etc")[6]], "the packed filesystem has the wrong shape: " + str(top[:5])
print("filesystem index ok:", len(top), "top-level entries")
PY
cp "$W/v86pkg/package/build/libv86.js" "$W/v86pkg/package/build/libv86.mjs" "$W/v86pkg/package/build/v86.wasm" "$OUT/"   # (libv86.mjs is for the Node test)
cp "$W/v86src/bios/seabios.bin" "$W/v86src/bios/vgabios.bin" "$OUT/"
cp "$W/kernel/buildroot-bzimage.bin" "$OUT/bzimage.bin"
# what the machine is made of, for the licences (GPL software: gcc, binutils, bash, coreutils, busybox, the kernel...)
{
  echo "The browser edition of c-cellar runs a small Linux computer inside the page (v86 emulator, Alpine Linux, gcc)."
  echo "Everything below is free software; the sources are available from the places named here."
  echo
  echo "v86 (the x86 emulator): BSD-2-Clause, https://github.com/copy/v86 (commit $(git -C "$W/v86src" rev-parse HEAD 2>/dev/null || echo unknown))"
  echo "Linux kernel (buildroot-bzimage.bin, built by the author of v86): GPL-2.0, https://github.com/copy/v86 and https://www.kernel.org/"
  echo "Alpine Linux userland: each package under its own licence; sources: https://gitlab.alpinelinux.org/alpine/aports (v3.21), https://dl-cdn.alpinelinux.org/alpine/v3.21/main/"
  echo
  echo "Packages installed in the machine (name, version, licence):"
  python3 - "$W/alpine/root/lib/apk/db/installed" <<'PY'
import sys
for line in open(sys.argv[1], encoding="utf-8", errors="replace").read().split(chr(10) * 2):
    d = {l[0]: l[2:] for l in line.splitlines() if len(l) > 2 and l[1] == ":"}
    if "P" in d:
        print(f"  {d['P']} {d.get('V', '')} {d.get('L', '')}")
PY
} > "$OUT/NOTICE.txt"
cp "$W/v86src/LICENSE" "$OUT/LICENSE-v86.txt" 2>/dev/null || true
echo "machine ready in $OUT: $(du -sh "$OUT" | cut -f1), $(ls "$OUT/rootfs/flat" | wc -l) files, biggest $(ls -S "$OUT/rootfs/flat" | head -1 | xargs -I{} stat -c %s "$OUT/rootfs/flat/{}") bytes"
