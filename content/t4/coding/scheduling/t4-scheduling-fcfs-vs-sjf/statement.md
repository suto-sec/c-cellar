# The convoy effect

All the jobs arrive at time 0. Compute the **average waiting time** if they are run first-come-first-served (in the order given) and if they are run shortest-job-first.

**Input**: the number of jobs `N` (1 to 50) on the first line, then `N` durations, one per line.

**Output**: two lines, `FCFS: A` and `SJF: B`, each with the mean waiting time to two decimals.

**Example** (the convoy example of the slides: a long job first)

```
$ printf '3\n24\n3\n3\n' | ./prog
FCFS: 17.00
SJF: 3.00
```
