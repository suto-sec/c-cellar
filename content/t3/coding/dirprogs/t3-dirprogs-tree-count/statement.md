# Count a tree

Write `treecount DIR`, which counts what is inside `DIR`, **at any depth**, and prints two lines:

```
files: <number>
directories: <number>
```

Every entry that is a directory counts as a directory (`DIR` itself is not counted) and every other entry counts as a file. Hidden entries count too. The number of entries can be large.

- `DIR` that cannot be opened (or is not a directory): `treecount: DIR: <reason>` (`strerror(errno)`) to standard error and status 1. Wrong number of arguments: `usage: treecount DIR` and status 2.

**Example**

```
$ ./prog proj
files: 14
directories: 3
```
