# A counter with O_RDWR

The file `FILE` contains a number and a line break. `./prog FILE` adds 1 to it, writes the new number (and a line break) **over the old one in the same file**, and prints the new value. Use a single descriptor opened with `O_RDWR`, `read`, `lseek` and `write` (and `snprintf` to build the text). The numbers never get shorter. If the file cannot be opened, exit with status 1.

**Example**

```
$ cat n.txt
7
$ ./prog n.txt
8
$ cat n.txt
8
```
