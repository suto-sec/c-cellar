# From a descriptor to a stream

`./prog FILE` prints `lines = <number of lines of FILE>`. Open the file with the system call `open`, then turn the descriptor into a stream with `fdopen` and read the lines with `fgets` (lines have fewer than 200 characters). If the file cannot be opened, print nothing and exit with status 1.

**Example**

```
$ cat f.txt
one two
three

$ ./prog f.txt
lines = 3
```
