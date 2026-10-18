# Permission report

Write `perms PATH...`. For every argument, in order, print one line with what the process can do with it, checked with `access(2)`:

```
PATH: r w x
```

where each of the three positions is the letter if the permission is granted (`r` read, `w` write, `x` execute/search) or `-` if it is not. A path that does not exist prints `PATH: missing`. The status is 0 in every case. Without arguments print `usage: perms PATH...` to standard error and exit with status 2.

**Example**

```
$ ./prog script.sh notes.txt nothing
script.sh: r w x
notes.txt: r - -
nothing: missing
```
