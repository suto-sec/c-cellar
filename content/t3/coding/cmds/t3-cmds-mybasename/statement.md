# mybasename

Write `mybasename PATH [SUFFIX]`, which behaves like `basename(1)`: it prints the last component of `PATH`.

- Trailing slashes are ignored: `a/b//` gives `b`. A path made only of slashes (`/`, `//`) gives `/`.
- If `SUFFIX` is given and the name ends with it, remove it, unless that would leave nothing (the name equal to the suffix stays).
- Wrong number of arguments: print `usage: mybasename PATH [SUFFIX]` to standard error and exit with status 2.

**Examples**

```
$ ./prog /usr/bin/gcc
gcc
$ ./prog src/main.c .c
main
```
