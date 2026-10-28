# SJF with preemption

Simulate **shortest job first, preemptive** (shortest remaining time first): at every moment the CPU runs the ready process with the **least time left**. When a process arrives with less remaining time than the running one, it takes the CPU.

**Input** (standard input). The first line has the number of processes `N` (from 1 to 50). Then come `N` lines, one per process, with two numbers: the **arrival time** and the **duration**. All times are whole milliseconds. The processes are called `P1`, `P2`, ... in the order they appear.

**Output**: `Pk: W` (waiting time = time spent ready but not running) and then `average: A` (two decimals).

**Ties** (equal remaining time): the process that arrived first, then the one listed first.

**Example** (the preemptive example of the slides)

```
$ printf '4\n0 7\n2 4\n4 1\n5 4\n' | ./prog
P1: 9
P2: 1
P3: 0
P4: 2
average: 3.00
```
