# A tiny rm

`./prog PATH...` deletes every path it is given with `remove`. If one fails, print `remove <path>: <message of errno>` to the error output, go on with the others, and exit with status 1 at the end (0 if all went well).

**Example**

```
$ ./prog a.txt nofile
remove nofile: No such file or directory
```
