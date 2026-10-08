# Count the entries

`countentries DIR` prints how many entries the directory has, **not counting** `.` and `..` (hidden files count). If `DIR` cannot be opened print `DIR: <reason>` on standard error and exit with 1.

**Example**

```
$ ./prog d
2
```
