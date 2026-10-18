# Adding times

Write `sumtimes`, which reads **standard input**, one duration per line in the form `H:M:S` (non-negative integers; `M` and `S` can be larger than 59), and prints the total, normalized, as `H:MM:SS` (hours without padding, minutes and seconds with two digits). Define a `struct` for a time and a function that adds two of them.

- Blank lines are ignored. With no durations the total is `0:00:00`.
- A line that is not of the form `H:M:S` prints `sumtimes: line N: not a time` to standard error (`N` counts all lines, starting at 1) and the program exits with status 1 without printing a total.

**Example**

```
$ printf '1:30:45\n0:45:30\n' | ./prog
2:16:15
```
