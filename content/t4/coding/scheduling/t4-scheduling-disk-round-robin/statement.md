# Round robin with a disk

Simulate **round robin** with a disk.

**The machine.** One CPU and one disk, both used by one process at a time. Times are whole milliseconds. Every process alternates **CPU bursts and disk bursts** and always starts and ends with a CPU burst (`CPU, disk, CPU, disk, ..., CPU`). The disk serves its requests **first come, first served** and never interrupts a burst. A process that is using the disk does not need the CPU, so the CPU can run another one meanwhile.

**What happens at the same instant.** When several things happen at time `t`, handle them in this order: (1) a disk burst that ends: its process becomes ready; (2) a CPU burst that ends: the process asks for the disk (or terminates, if it was its last burst); (3) new arrivals become ready, in the order they are listed; (4) the disk takes the first request waiting, if it is free; (5) the CPU is assigned.

**CPU scheduling.** FIFO ready queue and a quantum `Q`. The first ready process runs until its CPU burst ends or `Q` ms have passed. When the quantum is over, the process goes to the end of the ready queue (behind the ones that became ready at that instant). Every time a process takes the CPU it gets a **fresh quantum**, also if it comes back from the disk.

**Input** (standard input). The first line has `N Q` (the number of processes and the quantum); `N` is from 1 to 10. Then come `N` lines, one per process, the processes being called `P1`, `P2`, ... in the order they are listed. Each line is `arrival K burst1 burst2 ... burstK`: `K` (from 1 to 9, always odd) bursts that alternate CPU, disk, CPU, ..., CPU.

**Output**: `Pk: finish F ready R disk-wait W` per process (`F` instant at which it terminates, `R` ms spent ready, `W` ms spent waiting for the disk), then `average turnaround: A` (average of `finish - arrival`, two decimals).

**Example**

```
$ printf '2 30\n10 1 40\n0 7 15 5 20 10 30 10 20\n' | ./prog
P1: finish 75 ready 25 disk-wait 0
P2: finish 135 ready 25 disk-wait 0
average turnaround: 100.00
```
