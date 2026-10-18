# The biggest file

Write `biggest DIR`, which finds the **largest regular file** in `DIR` and its subdirectories (at any depth, hidden files included) and prints its path and its size in bytes, separated by a space: `PATH SIZE`. The path starts with `DIR` as it was given. If two files have the same size, the one whose path is smaller (`strcmp`) wins. Measure each file with `open` and `lseek`.

- `DIR` without any file: print `biggest: no files in DIR` to standard error and exit with status 1.
- `DIR` that cannot be opened: `biggest: DIR: <reason>` (`strerror(errno)`) to standard error and status 1. Wrong number of arguments: `usage: biggest DIR` and status 2.

**Example**

```
$ ./prog proj
proj/data/big.bin 20480
```
