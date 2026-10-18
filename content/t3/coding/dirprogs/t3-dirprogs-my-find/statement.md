# myfind

Write `myfind DIR NAME`, which prints the path of every entry (file **or** directory) called exactly `NAME` inside `DIR`, at any depth. A path is `DIR` (exactly as given), a `/`, and the names of the directories down to the entry, e.g. `proj/src/main.c`. `DIR` itself is not examined. The order does not matter. Nothing is printed if nothing is found (status 0).

- `DIR` that cannot be opened: `myfind: DIR: <reason>` (`strerror(errno)`) to standard error and status 1. Wrong number of arguments: `usage: myfind DIR NAME` and status 2.

**Example**

```
$ ./prog proj a.txt
proj/a.txt
proj/sub/more/a.txt
```
