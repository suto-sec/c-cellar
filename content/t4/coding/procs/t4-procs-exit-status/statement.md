# Report the exit status

`prog N` creates a child that ends with `exit(N)`. The parent waits for it and prints `child exited with N`, using the status it received from `wait` (do not just print `N` again).

**Examples**

```
$ ./prog 7
child exited with 7
$ ./prog 0
child exited with 0
```
