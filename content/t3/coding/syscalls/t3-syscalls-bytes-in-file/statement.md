# Size with lseek

`bytes FILE` prints the size of the file in bytes: open it with `open`, move to the end with `lseek(fd, 0, SEEK_END)` (it returns the position) and close it. A file that cannot be opened prints `FILE: <reason>` (use `strerror(errno)`) on standard error and exits with 1.

**Example**

```
$ ./prog f.txt
6
```
