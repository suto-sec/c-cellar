# Three queues with feedback

Simulate a **multilevel feedback queue** with three queues and a disk, and print the state diagram.

**The machine.** One CPU and one disk, both used by one process at a time. Times are whole milliseconds. Every process alternates **CPU bursts and disk bursts** and always starts and ends with a CPU burst (`CPU, disk, CPU, disk, ..., CPU`). The disk serves its requests **first come, first served** and never interrupts a burst. A process that is using the disk does not need the CPU, so the CPU can run another one meanwhile.

**What happens at the same instant.** When several things happen at time `t`, handle them in this order: (1) a disk burst that ends: its process becomes ready; (2) a CPU burst that ends: the process asks for the disk (or terminates, if it was its last burst); (3) new arrivals become ready, in the order they are listed; (4) the disk takes the first request waiting, if it is free; (5) the CPU is assigned.

**CPU scheduling.** Queue 0 is round robin with quantum `Q0`, queue 1 is round robin with quantum `Q1`, queue 2 is FCFS. Every new process starts in queue 0. The CPU takes the first process of the lowest-numbered non-empty queue, and a process that becomes ready in a higher queue **preempts** the running one: the preempted process goes to the **head** of its own queue and remembers what was left of its quantum. A process that uses its whole quantum goes to the **end of the next queue** (queue 2 is the last) with a fresh quantum. A process that leaves for the disk before its quantum is over **stays in its queue** and gets a fresh quantum when it comes back.

**Input** (standard input). The first line has `N Q0 Q1` (the number of processes and the two quanta); `N` is from 1 to 10. Then come `N` lines, one per process, the processes being called `P1`, `P2`, ... in the order they are listed. Each line is `arrival K burst1 burst2 ... burstK`: `K` (from 1 to 9, always odd) bursts that alternate CPU, disk, CPU, ..., CPU.

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
$ printf '2 20 40\n10 7 60 10 60 10 60 10 15\n5 1 30\n' | ./prog
P1: ..........LLLLLLLLLLLLLLLEEEEEEEEEEEEEEEEEEEELLLLLLLLLLEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEDDDDDDDDDDEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEDDDDDDDDDDEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEDDDDDDDDDDEEEEEEEEEEEEEEE
P2: .....EEEEEEEEEEEEEEEEEEEELLLLLLLLLLLLLLLLLLLLEEEEEEEEEETTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT
```
