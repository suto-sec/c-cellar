# Sort the lines of a file

`sortlines FILE` prints the lines of the file in alphabetical order (the order of `strcmp`), one per line, keeping duplicates. The file can have **any number of lines** (up to 1000 characters each), so allocate memory dynamically. A missing file prints `cannot open FILE` on standard error and exits with 1.

**Example**

```
$ ./prog f.txt
apple
fig
pear
```
