# mycmp

Write `mycmp FILE1 FILE2`, which compares two files byte by byte, like `cmp(1)`:

- Identical files: print nothing, exit with status 0.
- At the first differing byte print `FILE1 FILE2 differ: byte N, line M` to **standard output** and exit with status 1. `N` is the position of the byte (the first byte is 1) and `M` is the line it is on, which is 1 plus the number of newlines before it.
- If one file ends first (it is a prefix of the other) print `mycmp: EOF on FILE after byte N, line M` to **standard error**, where `FILE` is the shorter one, `N` the number of bytes it has and `M` is 1 plus the number of newlines among them. Exit with status 1.
- If a file cannot be opened print `mycmp: FILE: <reason>` (`strerror(errno)`) to standard error and exit with status 2. With the wrong number of arguments print `usage: mycmp FILE1 FILE2` to standard error and exit with status 2.

Files can contain any byte, including 0.

**Example**

```
$ printf 'abc\ndef\n' > a; printf 'abc\ndxf\n' > b
$ ./prog a b
a b differ: byte 6, line 2
```
