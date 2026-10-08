# A counter in a text file

The text file `FILE` contains a number and a line break. `./prog FILE` adds 1 to that number, stores it back **in the same file** (replacing the old one) and prints the new value. Open the file only once, with the mode `"r+"`. If the file cannot be opened, exit with status 1.

**Example**

```
$ cat counter.txt
41
$ ./prog counter.txt
42
$ cat counter.txt
42
```
