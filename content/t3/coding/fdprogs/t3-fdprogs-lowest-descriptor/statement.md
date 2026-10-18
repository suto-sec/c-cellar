# Which descriptor?

Write `fdnums FILE`, which does the following with `FILE` (always in this order) and prints the descriptor numbers it gets:

1. `a = open(FILE, O_RDONLY)`, `b = open(FILE, O_RDONLY)`. Print `a=<a>` and `b=<b>`.
2. `close(a)`.
3. `c = open(FILE, O_RDONLY)`, `d = dup(b)`, `e = dup2(b, 10)`, `f = open(FILE, O_RDONLY)`. Print `c=`, `d=`, `e=`, `f=` in that order.

One line each. The numbers are not typed in your program: they are what the system returns.

If `FILE` cannot be opened print `fdnums: FILE: <reason>` (`strerror(errno)`) to standard error and exit with status 1. With a wrong number of arguments print `usage: fdnums FILE` and exit with status 2.
