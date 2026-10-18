# Read one byte with read

Write `firstbyte FILE`, which opens the file with `open`, reads its **first byte** with `read` and prints `first byte: X` (where `X` is that character). If the file is empty print `empty`.

- A file that cannot be opened: print `cannot open FILE` and exit with status 1. A wrong number of arguments: status 2.

## Functions for this exercise

- `ssize_t read(int fd, void *buf, size_t count)` (in `<unistd.h>`) reads up to `count` bytes from the descriptor into `buf` and returns how many it read: `0` means the end of the file, `-1` an error.

```c
char c;
ssize_t n = read(fd, &c, 1);   /* one byte, into c */
if (n == 1)
    printf("%c\n", c);
```
