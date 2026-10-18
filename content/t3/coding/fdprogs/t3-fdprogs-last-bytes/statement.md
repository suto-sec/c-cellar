# The last bytes of a file

Write `lastbytes FILE N`, which writes the last `N` **bytes** of `FILE` to standard output (all of the file if it has fewer). Use `open`, `lseek`, `read` and `write` (no `FILE *`). The file can be larger than any buffer and can contain any byte, including 0.

- `N` is a non-negative integer (`0` prints nothing). Otherwise, or with the wrong number of arguments, print `usage: lastbytes FILE N` to standard error and exit with status 2.
- A file that cannot be opened: `lastbytes: FILE: <reason>` (`strerror(errno)`) to standard error and status 1.

**Example**

```
$ printf 'abcdefgh' > f.txt
$ ./prog f.txt 3
fgh
```
