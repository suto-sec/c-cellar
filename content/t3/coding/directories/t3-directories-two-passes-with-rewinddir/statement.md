# Two passes over a directory

`./prog DIR` prints `<n> entries`, where `n` is the number of entries of `DIR` (not counting `.` and `..`), and then the names of the entries, one per line, in any order. Read the directory twice with a single `opendir`, going back to the start with `rewinddir`. If the directory cannot be opened, print nothing and exit with status 1.

**Example**

```
$ ls d
alpha
beta.txt
$ ./prog d
2 entries
alpha
beta.txt
```
