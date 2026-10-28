# The child waits for a signal

1. Install a handler for `SIGUSR1` **before** creating the child.
2. The child waits with `pause()`. When the signal arrives, the handler prints `child: got SIGUSR1` and ends the process with status `0`.
3. The parent sends `SIGUSR1` to the child with `kill`, waits for it and prints `parent: child exited with 0`.

**Example**

```
$ ./prog
child: got SIGUSR1
parent: child exited with 0
```
