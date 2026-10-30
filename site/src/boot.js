/* How the machine is entered. The prebuilt kernel (copy.sh's buildroot-bzimage) carries its own tiny system, which mounts the 9p files at /mnt and
 * gives a "~% " prompt; it does not boot from them. So the page waits for that prompt and types one line that mounts /proc, /dev... inside the
 * Alpine files and chroots into them (/etc/lab-boot.sh prints "lab ready" and starts bash). Shared by vm.js (browser) and tools/vm_smoke.mjs (Node). */
(function (root) {
  "use strict";
  const api = {
    prompt: "~% ",
    ready: "lab ready",
    cmdline: "tsc=reliable mitigations=off random.trust_cpu=on console=ttyS0",
    command: "[ -e /mnt/etc/lab-boot.sh ] || mount -t 9p host9p /mnt -o trans=virtio,version=9p2000.L,cache=loose; " +
      "mount -t proc proc /mnt/proc; mount -t sysfs sysfs /mnt/sys; mount -o bind /dev /mnt/dev; mkdir -p /mnt/dev/pts; " +
      "mount -t devpts devpts /mnt/dev/pts; mount -t tmpfs tmpfs /mnt/tmp; exec chroot /mnt /etc/lab-boot.sh",
    // One check: unpack the job on the in-memory disk (it enforces file permissions, and is fast) and run it as an ordinary user, not root, so that
    // "permission denied" happens like on the student's own computer; results go back to /lab where the page reads them. The finishing word is split
    // ($((1+0))) so that the echo of the typed line cannot be mistaken for it.
    job: "rm -rf /tmp/job && mkdir /tmp/job && tar -xf /lab/job.tar -C /tmp/job && chown -R lab:lab /tmp/job && " +
      "su lab -s /bin/bash -c 'cd /tmp/job && J=/tmp/job bash ctl/run.sh'; cp /tmp/job/results.tar /lab/results.tar; cd /lab; echo CELLAR-JOB-$((1+0))DONE",
    jobDone: "CELLAR-JOB-1DONE",
    jobTar: "/lab/job.tar",
    resultsTar: "/lab/results.tar",
  };
  if (typeof module !== "undefined" && module.exports) module.exports = api;
  else root.CellarBoot = api;
})(typeof window !== "undefined" ? window : globalThis);
