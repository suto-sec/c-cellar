# Count from FIRST to LAST

`countrange FIRST LAST [STEP]` prints the integers from `FIRST` to `LAST` (both included), separated by single spaces on one line.

- Without `STEP` the step is `1` if `FIRST <= LAST` and `-1` otherwise.
- If `STEP` is 0, or points away from `LAST` (positive while `FIRST > LAST`, negative while `FIRST < LAST`), print `bad step` to **standard error** and exit with status 1.
- With a wrong number of arguments print `usage: countrange FIRST LAST [STEP]` to standard error and exit with status 2.

**Examples**

```
$ ./prog 3 7
3 4 5 6 7
```

```
$ ./prog 10 1 -3
10 7 4 1
```
