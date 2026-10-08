# A small option parser

Parse the arguments: `-n N` sets a number (default 1), `-v` turns verbose on (default `no`), and every other argument is a file name. Options may appear in any order and mixed with the files. Print `n=<N> verbose=<yes|no> files=<names separated by spaces, or none>`.

**Example**

```
$ ./prog -n 3 -v a.txt b.txt
n=3 verbose=yes files=a.txt b.txt
```
