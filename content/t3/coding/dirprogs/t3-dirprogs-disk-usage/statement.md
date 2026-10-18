# Bytes in a tree

Write `dusum DIR`, which prints one number: the total size in **bytes** of all the files in `DIR` and its subdirectories, at any depth (hidden files included). Directories themselves add nothing. Measure each file with `open` and `lseek` (a file you cannot open counts as 0 bytes).

- `DIR` that cannot be opened: `dusum: DIR: <reason>` (`strerror(errno)`) to standard error and status 1. Wrong number of arguments: `usage: dusum DIR` and status 2.

**Example**

```
$ ./prog proj
4096
```
