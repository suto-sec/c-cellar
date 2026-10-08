# Overwrite bytes in place

`patch FILE OFFSET TEXT` overwrites the bytes of `FILE` starting at the position `OFFSET` with the characters of `TEXT`, **without** truncating the file: open it write-only, move with `lseek(fd, OFFSET, SEEK_SET)` and `write` the text.

**Example**

```
$ ./prog f 6 WORLD; cat f; echo
hello WORLD
```
