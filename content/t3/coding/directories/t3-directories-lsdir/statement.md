# lsdir: sorted directory listing

Write `lsdir DIR` that prints the names of the entries of directory `DIR`, one per line, **sorted** with `strcmp` order.

- Do **not** print `.` and `..`. Do print other hidden names (those starting with a dot).
- Use `opendir`, `readdir`, `closedir`. The order `readdir` gives is arbitrary, so you must store the names and sort them. Free what you allocate.
- An empty directory prints nothing and exits 0.
- If `DIR` cannot be opened (missing, or not a directory), print a message with `strerror(errno)` to stderr and exit with status 1.

**Example**

```
$ ./prog d
.hidden
apple
banana
cherry
```
