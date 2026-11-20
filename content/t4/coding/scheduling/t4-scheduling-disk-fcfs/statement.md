# FCFS with a disk

Simulate **first come, first served** on the CPU for processes that also use a disk.

**The machine.** One CPU and one disk, both used by one process at a time. Times are whole milliseconds. Every process alternates **CPU bursts and disk bursts** and always starts and ends with a CPU burst (`CPU, disk, CPU, disk, ..., CPU`). The disk serves its requests **first come, first served** and never interrupts a burst. A process that is using the disk does not need the CPU, so the CPU can run another one meanwhile.

**What happens at the same instant.** When several things happen at time `t`, handle them in this order: (1) a disk burst that ends: its process becomes ready; (2) a CPU burst that ends: the process asks for the disk (or terminates, if it was its last burst); (3) new arrivals become ready, in the order they are listed; (4) the disk takes the first request waiting, if it is free; (5) the CPU is assigned.

**CPU scheduling.** Non-preemptive FCFS: when the CPU is free it takes the ready process that has been ready longest.

**Input** (standard input). The first line has `N`; `N` is from 1 to 10. Then come `N` lines, one per process, the processes being called `P1`, `P2`, ... in the order they are listed. Each line is `arrival K burst1 burst2 ... burstK`: `K` (from 1 to 9, always odd) bursts that alternate CPU, disk, CPU, ..., CPU.

**Output**: `Pk: finish F ready R disk-wait W` per process (`F` instant at which it terminates, `R` ms spent ready, `W` ms spent waiting for the disk), then `average turnaround: A` (average of `finish - arrival`, two decimals).

**Example**

```
$ printf '2\n15 5 20 10 10 30 5\n5 7 10 30 5 30 10 5 5\n' | ./prog
P1: finish 120 ready 0 disk-wait 30
P2: finish 125 ready 0 disk-wait 25
average turnaround: 112.50
```
