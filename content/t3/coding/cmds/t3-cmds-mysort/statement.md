# mysort with options

Write `mysort [-r] [-n] [-u]`, which sorts the lines of **standard input** and prints them.

- Default order: `strcmp` of the whole line.
- `-n`: numeric order: compare the number at the start of each line (`strtol`; a line that does not start with a number counts as 0). Lines with the same number are ordered by `strcmp`.
- `-r`: reverse the final order.
- `-u`: print each group of identical adjacent lines (after sorting) only once.
- Options can be given separately (`-r -n`) or together (`-rn`), in any order. An unknown option letter prints `mysort: invalid option -- 'X'` to standard error and exits with status 2. Any argument that is not an option prints `usage: mysort [-r] [-n] [-u]` to standard error and exits with status 2.
- The input can have any number of lines, up to 1000 characters each.

**Example**

```
$ printf '10\n9\n100\n9\n' | ./prog -nu
9
10
100
```
