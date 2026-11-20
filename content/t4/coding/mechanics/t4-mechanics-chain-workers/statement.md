# An assembly line of processes

`prog N` (with `N` at least 1) builds an assembly line of `N` worker processes joined by pipes. Worker `k` (the first one is worker 1):

- reads `int` values (with `read`, `sizeof(int)` bytes each) from the pipe that comes from worker `k-1` until the end of the data;
- adds `k` to each value;
- writes the result (with `write`) to the pipe that goes to worker `k+1`.

The parent plays both ends of the line: it writes the integers `1`, `2`, `3`, `4` and `5` into the first pipe, **closes** it, reads the results that come out of the last worker until the end of the data, prints each one on its own line, and waits for all the workers.

So with `N` workers every value grows by `1 + 2 + ... + N`.

**Example**

```
$ ./prog 2
4
5
6
7
8
```
