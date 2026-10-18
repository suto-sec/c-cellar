# A small hex dump

Write `hexd FILE`, which prints the content of the file in hexadecimal, **16 bytes per line**. Each line is:

- the offset of its first byte as 8 lower-case hexadecimal digits, then `:`;
- then, for each byte of the line, a space and the byte as 2 lower-case hexadecimal digits.

The last line has fewer bytes if the size is not a multiple of 16 (no padding). An empty file prints nothing. Use `open` and `read`; the file can contain any byte.

- A file that cannot be opened: `hexd: FILE: <reason>` (`strerror(errno)`) to standard error and status 1. Wrong number of arguments: `usage: hexd FILE` and status 2.

**Example**

```
$ printf 'Hello, world! 0123456789' > f.bin
$ ./prog f.bin
00000000: 48 65 6c 6c 6f 2c 20 77 6f 72 6c 64 21 20 30 31
00000010: 32 33 34 35 36 37 38 39
```
