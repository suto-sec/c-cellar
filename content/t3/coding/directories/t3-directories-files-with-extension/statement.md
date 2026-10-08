# Files with an extension

`withext DIR EXT` prints, sorted alphabetically, the names of the entries of `DIR` that end with the extension `EXT` (for example `.c`), one per line. A name equal to the extension alone (like `.c`) does not count.

**Example**

```
$ ./prog d .c
main.c
util.c
```
