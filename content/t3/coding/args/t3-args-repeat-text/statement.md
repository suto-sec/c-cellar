# Repeat a text

`repeat N TEXT` prints `TEXT` on its own line `N` times. If the number of arguments is not exactly 2, or `N` is negative, print `usage: repeat N TEXT` to **standard error** and exit with status 2 (print nothing on standard output).

**Example**

```
$ ./prog 3 ha
ha
ha
ha
```
