# Threads share memory

The program has a global `int value` that starts at `1`. The main thread prints `before: 1`, creates a thread that sets `value` to `42`, joins it, and prints `after: 42`.

(Compare with `fork`: a child process would have changed only **its copy**.)

**Example**

```
$ ./prog
before: 1
after: 42
```
