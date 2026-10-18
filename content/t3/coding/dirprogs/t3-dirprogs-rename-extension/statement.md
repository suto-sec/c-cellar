# Change the extension of many files

Write `renameext DIR OLD NEW`, which renames every entry of `DIR` whose name ends in `.OLD` so that it ends in `.NEW` instead (`OLD` and `NEW` are given **without** the dot). `rename(2)` does the work.

- The name must be **longer** than `.OLD` (a file called `.txt` has no extension to change). Hidden files count.
- For each renamed entry print `OLDNAME -> NEWNAME` (names only, without `DIR`). The order does not matter.
- If an entry called `NEWNAME` already exists do **not** overwrite it: print `renameext: OLDNAME: target exists` to standard error, skip it, and exit with status 1 at the end (0 if there was no problem).
- `DIR` that cannot be opened: `renameext: DIR: <reason>` (`strerror(errno)`) to standard error and status 1. Wrong number of arguments: `usage: renameext DIR OLD NEW` and status 2.

**Example**

```
$ ls d
a.txt  b.txt  notes.md
$ ./prog d txt bak
a.txt -> a.bak
b.txt -> b.bak
$ ls d
a.bak  b.bak  notes.md
```
