# Write a file

`writelines FILE` creates (or replaces) the file `FILE` with exactly these three lines: `line 1`, `line 2` and `line 3`. If the file cannot be created, print `cannot create FILE` to standard error and exit with status 1. Print nothing on standard output.

**Example**

```
$ ./prog notes.txt && cat notes.txt
line 1
line 2
line 3
```
