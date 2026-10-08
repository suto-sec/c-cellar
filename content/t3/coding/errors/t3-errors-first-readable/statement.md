# The first readable file

`firstreadable FILE...` tries to open each file for reading, in order, and prints the name of the first one that can be opened. If none can, print `none readable` to standard output and exit with status 1.

**Example**

```
$ ./prog a.txt notes.txt z.txt
notes.txt
```
