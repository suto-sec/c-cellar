# Redirect stdout and stderr

Write `logboth FILE`. The program does this, in this order:

1. Print `starting` (with `printf`) on the **original** standard output.
2. Redirect both **standard output and standard error** to `FILE` (created with permissions `0644`, or truncated if it exists), from inside the program, with `dup2`. It is what the shell does for `> FILE 2>&1`.
3. Print `out line` on standard output and `err line` on standard error (with `printf` and `fprintf(stderr, ...)`).

In `FILE` the two lines must be there in **that order**, and `starting` must not be in it. Nothing is printed to the original standard error.

- If `FILE` cannot be opened print `logboth: FILE: <reason>` (`strerror(errno)`) to standard error and exit with status 1. Wrong number of arguments: `usage: logboth FILE` and status 2.

**Example**

```
$ ./prog log.txt
starting
$ cat log.txt
out line
err line
```
