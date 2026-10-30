#!/bin/sh
# Run by the page inside a chroot of the Alpine files (see src/boot.js): the kernel's own little system mounts them at /mnt.
export PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin
export HOME=/root LANG=C.UTF-8 TERM=xterm PS1='\w\$ '
hostname lab 2>/dev/null
ulimit -c 0
# an ordinary user for the student's programs (root would ignore file permissions); /tmp is in memory, which does enforce them
grep -q '^lab:' /etc/passwd || { echo 'lab:x:1000:1000:lab:/home/lab:/bin/bash' >> /etc/passwd; echo 'lab:x:1000:' >> /etc/group; }
mkdir -p /home/lab /tmp /lab
chmod 1777 /tmp
chown lab:lab /home/lab
cd /lab
echo "lab ready"
exec /bin/bash --login
