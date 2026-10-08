# cat with open, read and write

`catfile FILE` writes the content of the file to standard output using only `open`, `read`, `write` and `close` (a buffer of 256 bytes). On failure to open print `FILE: <reason>` on standard error and exit with 1.

**Example**

```
$ ./prog f.txt
a
b
```
