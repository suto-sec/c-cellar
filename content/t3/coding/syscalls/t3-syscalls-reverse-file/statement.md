# Print a file backwards

`reversefile FILE` prints the bytes of the file in reverse order (the last byte first), using `open`, `lseek` and one-byte `read` calls, **without** loading the whole file in memory. A missing file exits with status 1 and a message on standard error.

**Example**

```
$ ./prog f
desserts```
