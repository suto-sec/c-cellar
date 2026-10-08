# Size of a file

`filesize FILE` prints the size of the file in bytes, using `fseek` to go to the end and `ftell` to read the position. A missing file prints `cannot open FILE` on standard error and exits with 1.

**Example**

```
$ ./prog f.txt
6
```
