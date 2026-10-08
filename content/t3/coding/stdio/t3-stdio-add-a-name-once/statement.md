# Add a name once

`./prog FILE NAME` appends the line `NAME` to `FILE` unless `FILE` already has exactly that line. It prints `added` or `already there`. The file may not exist yet. Open it only once, with the mode `"a+"` (names have fewer than 100 characters).

**Example**

```
$ ./prog names.txt ann
added
$ ./prog names.txt ann
already there
```
