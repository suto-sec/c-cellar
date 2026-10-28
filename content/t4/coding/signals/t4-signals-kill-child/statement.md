# Kill a child

The parent creates a child that does nothing forever (`for (;;) pause();`). The parent sends it `SIGTERM` with `kill` and waits for it. Then it prints `child was killed` if the child did **not** end normally (with `WIFEXITED`), or `child exited` if it did.

**Example**

```
$ ./prog
child was killed
```
