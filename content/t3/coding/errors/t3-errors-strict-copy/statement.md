# A copy that checks everything

`strictcopy SRC DST` copies a file with `fopen`/`fread`/`fwrite`, checking **every** call. Exit with: `0` on success; `1` if `SRC` cannot be opened (print `cannot open SRC: <reason>` on stderr); `2` if `DST` cannot be created (print `cannot create DST: <reason>` on stderr, and close `SRC`); `3` if the number of arguments is not 2 (print `usage: strictcopy SRC DST`). Print nothing on stdout.

**Example**

```
$ ./prog a b; echo "rc=$?"
rc=0
```
