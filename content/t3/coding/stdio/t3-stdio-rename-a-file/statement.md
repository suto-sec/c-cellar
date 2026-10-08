# A tiny mv

`./prog OLD NEW` renames the file `OLD` to `NEW` with `rename`. If it fails, call `perror("rename")` and exit with status 1. If the number of arguments is not 2, exit with status 1 without printing anything.

**Example**

```
$ ./prog notes.txt old-notes.txt
$ ls
old-notes.txt
```
