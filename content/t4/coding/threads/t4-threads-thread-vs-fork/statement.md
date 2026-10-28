# Thread or process?

The program has a global `int x` that starts at `1`.

1. A **thread** sets `x` to `2`. After joining it, the main thread prints `after thread: x = 2`.
2. Then a **child process** (`fork`) sets `x` to `3`. After waiting for it, the main thread prints `after child process: x = 2`.

Same assignment, different result: threads share memory, processes copy it.

**Example**

```
$ ./prog
after thread: x = 2
after child process: x = 2
```
