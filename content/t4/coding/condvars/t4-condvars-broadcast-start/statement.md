# Starting gun

Five worker threads each announce that they are **waiting** and sleep on a condition until the main thread fires the starting gun. The main thread:

1. waits until all 5 workers are waiting;
2. sets `go = 1` and wakes **all** of them at once (`pthread_cond_broadcast`);
3. joins them, and prints `workers started: 5`.

Each worker, once released, adds 1 to a shared counter `started` (under the mutex). The final line must show that all 5 got through.

**Example**

```
$ ./prog
workers started: 5
```
