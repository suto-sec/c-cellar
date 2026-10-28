# Wake me in a second

1. Install a handler for `SIGALRM` that prints `alarm!`.
2. Print `waiting`.
3. Ask for an alarm in one second with `alarm(1)` and sleep with `pause()`.
4. When `pause` returns, print `done`.

**Example**

```
$ ./prog
waiting
alarm!
done
```

Careful: the three lines must come out in that order even when the output goes through a pipe.
