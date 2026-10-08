# findexe

Write `findexe NAME` that searches every directory of the `PATH` environment variable, in order, for an **executable** file called `NAME`.

- Print `dir/NAME` (exactly that string) for **each** match, one per line, in `PATH` order.
- A file that exists but is not executable does not count.
- Exit with status 0 if at least one match was printed, 1 otherwise (also if `PATH` is unset).
- With the wrong number of arguments print a usage message to stderr and exit with status 2.

```
$ PATH=/usr/local/bin:/usr/bin ./prog ls
/usr/bin/ls
```

Do not modify the string returned by `getenv`.
