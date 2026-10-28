# Predict the next CPU burst

SJF needs to know the length of the next CPU burst, which is unknown, so it is **estimated** from the past with an exponential average:

`tau(n+1) = alpha * t(n) + (1 - alpha) * tau(n)`

where `t(n)` is the real length of burst `n` and `tau(n)` the estimate that was made for it.

**Input**: first line `alpha` (a decimal) and the first estimate `tau1`; second line `N` (the number of real bursts, 0 to 50); then `N` real burst lengths (on one line or several).

**Output**: for each burst `n = 1..N` one line `n: E` with the estimate that had been made **before** the burst, then a last line `next: E` with the estimate for the following burst. All numbers with two decimals.

**Example**

```
$ printf '0.5 10\n4\n6 4 6 4\n' | ./prog
1: 10.00
2: 8.00
3: 6.00
4: 6.00
next: 5.00
```
