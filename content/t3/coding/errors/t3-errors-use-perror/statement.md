# Use perror

Same as before but shorter: `tryopen FILE` prints `opened FILE` on success. On failure call `perror(FILE)` (which writes `FILE: <reason>` on standard error) and exit with status 1.

**Example**

```
$ ./prog nope 2>&1; echo "rc=$?"
nope: No such file or directory
rc=1
```
