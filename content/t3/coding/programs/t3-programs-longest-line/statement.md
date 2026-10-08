# The longest line

`longestline FILE` prints the length and the text of the longest line of the file, in the format `<length>: <line>` (the length does not include the newline; if several lines tie, the first one wins). Lines are at most 1000 characters. An empty file prints `empty`. A missing file prints `cannot open FILE` on standard error and exits with 1.

**Example**

```
$ ./prog f.txt
8: elephant
```
