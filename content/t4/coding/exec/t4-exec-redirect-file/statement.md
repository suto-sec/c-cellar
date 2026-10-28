# Send a command's output to a file

`prog FILE CMD [ARGS...]` runs `CMD` in a child with its standard output redirected to `FILE` (created, or emptied if it exists; permissions `0644`), like `CMD ARGS > FILE` in a shell. The parent waits and prints `saved`.

**Example**

```
$ ./prog out.txt echo hello
saved
$ cat out.txt
hello
```
