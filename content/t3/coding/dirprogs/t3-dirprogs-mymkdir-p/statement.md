# mymkdir -p

Write `mymkdirp PATH`, which creates the directory `PATH` **and any missing parent directories** (like `mkdir -p`), with mode `0755`. Absolute and relative paths are allowed, and a trailing `/` is fine.

- A directory that already exists is not an error (and is not modified).
- If some component exists but is **not a directory** (a file), or a directory cannot be created, print `mymkdirp: PATH: <reason>` to standard error, where PATH is the argument exactly as given and the reason is `strerror(errno)` (`Not a directory` for the file case, `Permission denied`, ...), and exit with status 1.
- Wrong number of arguments: `usage: mymkdirp PATH` and status 2.

**Example**

```
$ ./prog a/b/c
$ find a
a
a/b
a/b/c
```
