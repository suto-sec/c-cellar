# Handle once, then die

The parent creates a child. The child installs a `SIGINT` handler that prints `first` (with `write`) and puts back the default action (`SIG_DFL`). The child then does `raise(SIGINT)` **twice**, and ends with `exit(0)` if it is still alive.

The first `SIGINT` runs the handler; the second one kills the child. The parent waits and prints `child died on the second SIGINT` if the child did **not** end normally, or `child survived` if it did.

**Example**

```
$ ./prog
first
child died on the second SIGINT
```
