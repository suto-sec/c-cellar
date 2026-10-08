# Only the hidden files

`hidden DIR` prints the names of the hidden entries of `DIR` (names that start with a dot, but **not** `.` and `..`), sorted alphabetically (`strcmp` order), one per line. Print nothing if there are none.

**Example**

```
$ ./prog d
.env
.git
```
