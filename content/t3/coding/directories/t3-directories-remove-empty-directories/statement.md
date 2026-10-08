# A tiny rmdir

`./prog DIR...` removes every directory it is given with `rmdir`. For each failure print `rmdir <path>: <message of errno>` to the error output and go on; exit with status 1 at the end if there was any failure (0 otherwise).

**Example**

```
$ ./prog empty full
rmdir full: Directory not empty
```
