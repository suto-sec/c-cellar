# What survives an exec

The command `sh -c 'kill -INT $$; exit 5'` sends `SIGINT` to **itself** and, if it is still alive, ends with status 5.

The program runs that command three times, each time in a new child created with `fork` and `execlp`, and prints what happened. Before each run the parent sets the disposition of `SIGINT` in itself:

| label | in the parent | in the child before `exec` |
|---|---|---|
| `ignored` | `SIG_IGN` | nothing |
| `caught` | a handler of your own (an empty function) | nothing |
| `restored` | `SIG_IGN` | `signal(SIGINT, SIG_DFL)` |

After each run the parent prints `LABEL: child exited with N` or `LABEL: child killed by signal N`.

**Example**

```
$ ./prog
ignored: child exited with 5
caught: child killed by signal 2
restored: child killed by signal 2
```
