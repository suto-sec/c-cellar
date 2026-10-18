# fopen modes with open

Write `fmode MODE FILE TEXT`, which does what `fopen(FILE, MODE)` followed by writing `TEXT` would do, but with `open(2)` and `write(2)`:

| MODE | meaning |
|---|---|
| `r` | read only; the file must exist |
| `w` | write only; create it or **truncate** it |
| `a` | write only; create it if needed; writes go at the **end** |
| `r+` | read and write; the file must exist; the position starts at the beginning (nothing is truncated) |
| `w+` | read and write; create or truncate |
| `a+` | read and write; create if needed; writes go at the end |

New files get permissions `0644`. After opening, write the bytes of `TEXT` (without a newline) to the file with `write` and close it. Print nothing on success.

- If `open` fails print `fmode: FILE: <reason>` (`strerror(errno)`) to standard error and exit with status 1.
- If `write` fails (for example in mode `r`, where the descriptor is read only) print `fmode: write: <reason>` to standard error and exit with status 1.
- An unknown mode or a wrong number of arguments: `usage: fmode MODE FILE TEXT` to standard error and status 2.

**Example**

```
$ printf abcdef > f.txt
$ ./prog r+ f.txt XY
$ cat f.txt
XYcdef
```
