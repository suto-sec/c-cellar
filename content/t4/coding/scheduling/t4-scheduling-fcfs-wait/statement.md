# FCFS: waiting times

Simulate **first come, first served** scheduling with no I/O (each process has one CPU burst) and print how long every process waits in the ready queue.

**Input** (standard input). The first line has the number of processes `N` (from 1 to 50). Then come `N` lines, one per process, with two numbers: the **arrival time** and the **duration** of the CPU burst. All times are whole milliseconds. The processes are called `P1`, `P2`, ... in the order they appear.

**Output**: one line per process, `Pk: W`, with `W` the waiting time (the time between its arrival and the moment it starts running); then `average: A` with the mean waiting time to two decimals.

Processes that arrive at the same time run in the order they are listed.

**Example** (the example of the course slides)

```
$ printf '3\n0 24\n0 3\n0 3\n' | ./prog
P1: 0
P2: 24
P3: 27
average: 17.00
```
