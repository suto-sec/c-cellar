# Copy with fread and fwrite

`copyblocks SRC DST` copies a file using `fread` and `fwrite` with a buffer of 512 bytes, then prints `copied N bytes` (N is the size of the file). If `SRC` cannot be opened print `cannot open SRC` to standard error and exit with 1.

**Example**

```
$ ./prog a.txt b.txt
copied 5 bytes
```
