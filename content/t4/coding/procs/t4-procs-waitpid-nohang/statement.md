# Not finished yet

The parent creates a child that sleeps for one second (`sleep(1)`) and then ends with status `5`.

1. Right after creating it, the parent asks `waitpid(pid, &status, WNOHANG)`. The child is still running, so it prints `child still running`.
2. Then the parent waits for the child normally and prints `child exited with 5`.

**Example**

```
$ ./prog
child still running
child exited with 5
```
