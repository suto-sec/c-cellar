# Count the lines of many files at once

`prog FILE...` (up to 16 files) counts the lines of every file **in parallel**: the parent creates one child per file, each with its own pipe. A child counts the newline characters of its file (like `wc -l`) and sends the number to the parent through its pipe.

The parent then reads the answers in the order of the arguments and prints `FILE: N` for each, and finally `total: T`, the sum. A file that cannot be opened prints `FILE: error` and does not count in the total.

**Example**

```
$ ./prog a.txt missing.txt b.txt
a.txt: 3
missing.txt: error
b.txt: 5
total: 8
```
