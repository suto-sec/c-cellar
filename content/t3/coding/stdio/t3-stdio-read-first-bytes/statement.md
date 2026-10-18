# Read a block with fread

Write `firstbytes FILE`, which reads **up to 8 bytes** from the start of the file with a single `fread` and prints:

```
read N bytes: TEXT
```

where `N` is the number of bytes obtained and `TEXT` is those bytes. (The files in the tests are text without newlines in the first 8 bytes.)

- A file that cannot be opened: print `cannot open FILE` to standard error and exit with status 1. A wrong number of arguments: status 2.

## Functions for this exercise

- `size_t fread(void *buf, size_t size, size_t count, FILE *f)` reads up to `count` items of `size` bytes into `buf` and returns **how many items** it read. With `size` 1, the result is the number of bytes.

```c
char buf[9];
size_t n = fread(buf, 1, 8, f);
buf[n] = '\0';            /* now it is a string */
```
