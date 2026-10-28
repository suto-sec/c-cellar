# SJF without preemption

Simulate **shortest job first, cooperative** (non-preemptive): whenever the CPU is free, the ready process with the **shortest duration** runs until it finishes.

**Input** (standard input). The first line has the number of processes `N` (from 1 to 50). Then come `N` lines, one per process, with two numbers: the **arrival time** and the **duration**. All times are whole milliseconds. The processes are called `P1`, `P2`, ... in the order they appear.

**Output**: `Pk: W` with the waiting time of each process, then `average: A` (two decimals).

**Ties**: between processes of the same duration, the one that arrived first goes first; if they arrived together, the one listed first.

**Example** (the cooperative example of the slides)

```
$ printf '4\n0 7\n2 4\n4 1\n5 4\n' | ./prog
P1: 0
P2: 6
P3: 3
P4: 7
average: 4.00
```
