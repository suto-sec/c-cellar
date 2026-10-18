# myseq

Write `myseq`, which does what `seq(1)` does without options. It must understand only:

```
myseq LAST
myseq FIRST LAST
myseq FIRST INCREMENT LAST
```

It prints the numbers one per line. `FIRST` and `INCREMENT` default to 1. Use `atoi(3)` to convert the arguments.

- A **negative** `INCREMENT` counts down (`myseq 5 -2 1` prints 5, 3, 1).
- If the sequence is empty (for example `myseq 5 1`) it prints nothing and succeeds.
- `INCREMENT` 0 prints `myseq: the increment cannot be 0` to standard error and exits with status 1.
- Any other number of arguments prints `usage: myseq [FIRST [INCREMENT]] LAST` to standard error and exits with status 2.

**Examples**

```
$ ./prog 3
1
2
3
$ ./prog 2 5
2
3
4
5
$ ./prog 3 4 12
3
7
11
```
