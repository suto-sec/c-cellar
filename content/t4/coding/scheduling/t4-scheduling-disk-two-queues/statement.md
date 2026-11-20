# Two queues: FCFS above, RR below

Simulate a CPU with **two ready queues** and a disk, and print the state diagram.

**The machine.** One CPU and one disk, both used by one process at a time. Times are whole milliseconds. Every process alternates **CPU bursts and disk bursts** and always starts and ends with a CPU burst (`CPU, disk, CPU, disk, ..., CPU`). The disk serves its requests **first come, first served** and never interrupts a burst. A process that is using the disk does not need the CPU, so the CPU can run another one meanwhile.

**What happens at the same instant.** When several things happen at time `t`, handle them in this order: (1) a disk burst that ends: its process becomes ready; (2) a CPU burst that ends: the process asks for the disk (or terminates, if it was its last burst); (3) new arrivals become ready, in the order they are listed; (4) the disk takes the first request waiting, if it is free; (5) the CPU is assigned.

**CPU scheduling.** Queue 0 (high priority) is **FCFS**. Queue 1 (low priority) is **round robin** with quantum `Q`. A low process runs only if no high process is ready. If a high process becomes ready while a low one runs, it **preempts** it: the low process goes to the **head** of the low queue and remembers how much of its quantum was left. A process always stays in its queue; each time it comes back from the disk it gets a fresh quantum. A low process whose quantum is over goes to the end of the low queue with a fresh quantum.

**Input** (standard input). The first line has `N Q` (the number of processes and the quantum); `N` is from 1 to 10. Then come `N` lines, one per process, the processes being called `P1`, `P2`, ... in the order they are listed. Each line is `arrival priority K burst1 burst2 ... burstK`: `K` (from 1 to 9, always odd) bursts that alternate CPU, disk, CPU, ..., CPU.

**Output**: one line per process, `Pk: ` and one letter per millisecond from time 0 up to the instant at which the **last** process terminates (so all the lines have the same length).

The letters:

| letter | the process is |
|---|---|
| `.` | not arrived yet |
| `L` | ready: waiting for the CPU |
| `E` | running on the CPU |
| `W` | waiting for the disk (queued) |
| `D` | using the disk |
| `T` | terminated |

**Example**

```
$ printf '2 10\n10 1 3 40 20 20\n5 0 3 5 10 15\n' | ./prog
P1: ..........EEEEEEEEEELLLLLLLLLLLLLLLEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEDDDDDDDDDDDDDDDDDDDDEEEEEEEEEEEEEEEEEEEE
P2: .....EEEEEDDDDDDDDDDEEEEEEEEEEEEEEETTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT
```
