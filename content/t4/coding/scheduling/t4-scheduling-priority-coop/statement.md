# Priorities without preemption

Simulate **priority scheduling, cooperative**: when the CPU is free, the ready process with the **highest priority** runs until it finishes. A **smaller number** means a **higher priority** (`1` beats `3`).

**Input** (standard input). The first line has the number of processes `N` (from 1 to 50). Then come `N` lines, one per process, with three numbers: the **arrival time**, the **duration** and the **priority**. All times are whole milliseconds. The processes are called `P1`, `P2`, ... in the order they appear.

**Output**: `Pk: W` (waiting time) for each process, then `average: A` (two decimals).

**Ties** (same priority): the process that arrived first, then the one listed first.

**Example**

```
$ printf '3\n0 4 3\n0 2 1\n0 3 2\n' | ./prog
P1: 5
P2: 0
P3: 2
average: 2.33
```
