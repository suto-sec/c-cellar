# myls

Write `myls [-a] [DIR]`, which lists the entries of `DIR` (the current directory by default) one per line, sorted by name in `strcmp` order.

- Names that start with a dot are **hidden**: they are listed only with `-a`. The entries `.` and `..` are never listed.
- The name of a **directory** is followed by `/`.
- `-a` can come before or after `DIR`. Any other option, or more than one `DIR`, prints `usage: myls [-a] [DIR]` to standard error and exits with status 2.
- If `DIR` cannot be opened print `myls: DIR: <reason>` (`strerror(errno)`) to standard error and exit with status 1.

**Example**

```
$ ls -A proj
.git  Makefile  src  README
$ ./prog -a proj
.git/
Makefile
README
src/
```
