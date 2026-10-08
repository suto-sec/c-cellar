# copyfile

Write `copyfile SRC DST` that copies the file `SRC` to `DST` using the **system calls** `open`, `read`, `write` and `close` (not `fopen`/`fread`).

- If `DST` exists it is emptied first; if it does not exist it is created with permissions `0644`.
- Use a buffer smaller than the file (for example 256 bytes) and loop; the program must work for any file size.
- If `SRC` cannot be opened, print a message that includes `strerror(errno)` to **stderr**, create nothing and exit with status 1.
- With the wrong number of arguments print a usage message to stderr and exit with a non-zero status.
- Exit with status 0 on success. Close both descriptors.

```
$ ./prog notes.txt backup.txt && cat backup.txt
```
