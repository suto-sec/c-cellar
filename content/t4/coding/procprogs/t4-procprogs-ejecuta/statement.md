# ejecuta: run a command

Write `ejecuta CMD [ARGS...]`. It creates a child process that runs the command with its options (`execvp`).

- If the command cannot be run, the child prints `error: cannot run CMD: REASON` to **standard error** (the text of `strerror(errno)`) and ends with status 1.
- The parent waits for the child and then prints, on standard output, `the command finished correctly` if it ended normally with status 0, or `the command did not run correctly` in any other case (it failed to start, ended with a non-zero status, ...).

**Examples**

```
$ ./prog echo hello world
hello world
the command finished correctly
$ ./prog lss -l
the command did not run correctly
```

(In the second case the message about the missing command goes to standard error.)
