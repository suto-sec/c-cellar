# FCFS: turnaround times

Same simulation as *FCFS: waiting times*, but report the **turnaround time** of each process: the time from its arrival until it finishes (waiting plus running).

**Input** (standard input). The first line has the number of processes `N` (from 1 to 50). Then come `N` lines, one per process, with two numbers: the **arrival time** and the **duration** of the CPU burst. All times are whole milliseconds. The processes are called `P1`, `P2`, ... in the order they appear.

**Output**: one line per process, `Pk: T` with its turnaround time, then `average: A` with the mean to two decimals. Processes that arrive at the same time run in the order they are listed.

**Example**

```
$ printf '3\n0 24\n0 3\n0 3\n' | ./prog
P1: 24
P2: 27
P3: 30
average: 27.00
```
