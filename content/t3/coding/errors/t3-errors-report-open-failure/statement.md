# Report why it failed

`tryopen FILE` opens the file for reading. On success print `opened FILE` on standard output. On failure print `cannot open FILE: <reason>` on **standard error** (the reason is the text from `strerror(errno)`) and exit with status 1.

**Example**

```
$ ./prog missing.txt 2>&1; echo "rc=$?"
cannot open missing.txt: No such file or directory
rc=1
```
