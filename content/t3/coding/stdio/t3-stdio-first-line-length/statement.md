# Where am I in the file?

Write `linelength FILE`, which opens the file, reads its **first line** one character at a time and then prints `position: N`, where `N` is the position of the stream after reading that line (the newline included).

- If the file has no newline, read until the end of the file.
- A file that cannot be opened: print `cannot open FILE` to standard error and exit with status 1. A wrong number of arguments: status 2.

## Functions for this exercise

- `int fgetc(FILE *f)` reads one character; it returns `EOF` at the end of the file.
- `long ftell(FILE *f)` tells how many bytes from the start of the file the stream is at.

```c
int c;
while ((c = fgetc(f)) != EOF && c != '\n')
    ;                      /* nothing to do with c in this exercise */
printf("%ld\n", ftell(f));
```
