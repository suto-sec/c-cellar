# Collect children from a handler

`prog N` installs a handler for `SIGCHLD` and creates `N` children that end at once (`exit(0)`). The handler collects the finished children with `waitpid(-1, NULL, WNOHANG)` **in a loop** and counts them.

The parent does not call `wait` itself: it just sleeps until the handler has collected all `N` (`while (reaped < n) sleep(1);`) and prints `reaped N`.

**Example**

```
$ ./prog 3
reaped 3
```
