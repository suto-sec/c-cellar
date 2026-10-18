# The number behind a stream

Write a program that prints the descriptor number of each of the three standard streams:

```
stdin = 0
stdout = 1
stderr = 2
```

It takes no input and prints exactly these three lines (obtaining the numbers from the streams, not typing them).

## Functions for this exercise

- `int fileno(FILE *f)` (in `<stdio.h>`) returns the **descriptor** (the small integer that system calls such as `read` and `write` use) behind a `FILE *`. `stdin`, `stdout` and `stderr` are `FILE *` values that exist in every program.

```c
printf("%d\n", fileno(stdout));
```
