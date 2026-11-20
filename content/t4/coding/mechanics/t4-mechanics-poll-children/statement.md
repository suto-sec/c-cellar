# Keep working while they run

The parent creates three children, numbered 1, 2 and 3. Child `i` sleeps and then ends with exit status `i`:

| child | sleeps (`usleep`) |
|---|---|
| 1 | 750 ms |
| 2 | 250 ms |
| 3 | 500 ms |

The parent must **never block** in `wait`. It loops until the three have been collected: each round it calls `waitpid(-1, &status, WNOHANG)`.

- If a child was collected, print `child N finished` (`N` taken from its exit status).
- If none had finished yet, count one *poll* and sleep 50 ms (`usleep(50000)`).

At the end print `parent kept working` if it polled more than 5 times, or `parent was blocked` otherwise (a parent that waits with a blocking `wait` polls almost never).

**Example**

```
$ ./prog
child 2 finished
child 3 finished
child 1 finished
parent kept working
```
