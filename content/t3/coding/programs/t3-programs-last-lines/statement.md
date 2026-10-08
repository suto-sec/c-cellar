# The last lines

`lastlines [-N]` reads standard input and prints its **last `N` lines** (10 by default). With fewer than `N` lines it prints them all; `-0` prints nothing. Lines have at most 1000 characters. The input can be very long, so keep **only the last `N` lines** in memory (a circular buffer), not the whole input.

**Example**

```
$ seq 1 10 | ./prog -3
8
9
10
```
