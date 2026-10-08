# Save and restore stdout

`./prog FILE` writes the line `redirected` into `FILE` and the line `back on screen` to the **real** standard output, in this order and using only the system calls `open`, `dup`, `dup2`, `write` and `close`:

1. save descriptor 1 with `dup`;
2. open `FILE` (create it, empty it) and make it the new standard output with `dup2`;
3. `write` `redirected` to descriptor 1 (it goes into the file);
4. restore the original standard output with `dup2` and `write` `back on screen` to descriptor 1.

**Example**

```
$ ./prog out.txt
back on screen
$ cat out.txt
redirected
```
