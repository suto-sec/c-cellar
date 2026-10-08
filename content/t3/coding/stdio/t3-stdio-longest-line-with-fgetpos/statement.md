# Remember a position

`./prog FILE` prints the longest line of `FILE` (the first one if several have the same length; lines have fewer than 200 characters and the file has at least one line). Do not keep all the lines in memory: remember where the longest line starts with `fgetpos`, and when you reach the end go back there with `fsetpos` and read it again.

**Example**

```
$ cat lines.txt
short
the longest one here
mid size
$ ./prog lines.txt
the longest one here
```
