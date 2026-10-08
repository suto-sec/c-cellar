# List the PATH

`pathlist` prints the directories of the `PATH` environment variable, one per line, numbered from 1: `1 /usr/local/bin`. Empty entries (as in `a::b`) are skipped and do not use a number. If `PATH` is not set print `no PATH` and exit with status 1.

**Example**

```
$ env PATH=/bin:/usr/bin ./prog
1 /bin
2 /usr/bin
```
