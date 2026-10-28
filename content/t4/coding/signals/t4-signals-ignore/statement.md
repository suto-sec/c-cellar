# Ignore a signal

The program tells the system to **ignore** `SIGINT`, sends it to itself with `raise(SIGINT)` and prints `still alive`. Normally `SIGINT` (Ctrl+C) kills a program.

**Example**

```
$ ./prog
still alive
```
