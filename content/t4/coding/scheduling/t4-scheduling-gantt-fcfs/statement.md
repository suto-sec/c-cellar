# Draw the Gantt chart

Print the **Gantt chart** of an FCFS schedule (one CPU burst per process): one line per interval, `START-END NAME`, in time order. When the CPU has nothing to run until the next arrival, print that interval with the name `idle`.

**Input** (standard input). The first line has the number of processes `N` (from 1 to 50). Then come `N` lines, one per process, with two numbers: the **arrival time** and the **duration**. All times are whole milliseconds. The processes are called `P1`, `P2`, ... in the order they appear.

Processes that arrive at the same time run in the order they are listed. There is no `idle` line before time 0 or after the last process.

**Example**

```
$ printf '3\n0 3\n10 2\n11 4\n' | ./prog
0-3 P1
3-10 idle
10-12 P2
12-16 P3
```
