# Round robin

Simulate **round robin** with a fixed time quantum. Ready processes wait in a FIFO queue; the first one runs for at most one quantum; if it still has work left it goes to the back of the queue.

**Input** (standard input). The first line has the number of processes `N` (from 1 to 50) and the quantum `Q` (a second number on that same line). Then come `N` lines, one per process, with two numbers: the **arrival time** and the **duration**. All times are whole milliseconds. The processes are called `P1`, `P2`, ... in the order they appear.

**Output**: `Pk: W` with the waiting time of each process (finish time - arrival - duration), then `average: A` (two decimals).

**Order in the queue**: a process that arrives while another is running (or at the very instant the running one ends its quantum) is put in the queue **before** the process that has just been preempted. Processes that arrive together are queued in the order they are listed.

**Example** (the round-robin example of the slides, quantum 20)

```
$ printf '4 20\n0 53\n0 17\n0 68\n0 24\n' | ./prog
P1: 81
P2: 20
P3: 94
P4: 97
average: 73.00
```
