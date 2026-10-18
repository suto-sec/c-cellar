# dup shares the position, open does not

Write `sharedoffset FILE` (the file has at least 6 bytes). It does two experiments:

1. Open `FILE` as `fd1`, make `fd2 = dup(fd1)`, then read **3 bytes from `fd1`** and then **3 bytes from `fd2`**.
2. Open `FILE` twice more, as `fd3` and `fd4`, then read **3 bytes from `fd3`** and **3 bytes from `fd4`**.

Print what was read:

```
dup: <3 bytes from fd1> <3 bytes from fd2>
open: <3 bytes from fd3> <3 bytes from fd4>
```

(Use `read(2)` and print the bytes as text, with `%s` after you terminate them with `'\0'`.)

- A file that cannot be opened: `sharedoffset: FILE: <reason>` to standard error and status 1. A file with fewer than 6 bytes: `sharedoffset: FILE: too short` and status 1. Wrong number of arguments: `usage: sharedoffset FILE` and status 2.

**Example**

```
$ printf 'abcdefghi' > f.txt
$ ./prog f.txt
dup: abc def
open: abc abc
```
