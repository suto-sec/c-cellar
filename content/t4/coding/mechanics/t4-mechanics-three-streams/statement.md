# Three streams, three files

`prog IN OUT ERR` runs this fixed command in a child:

```
sh -c 'sort; echo sort done >&2'
```

with its **standard input** read from the file `IN`, its **standard output** written to `OUT` and its **standard error** written to `ERR` (both created, or emptied if they exist; permissions `0644`).

If any of the three files cannot be opened, the child prints `FILE: Error. REASON` to the real standard error (`REASON` is the text of the error, e.g. `No such file or directory`), does **not** run the command and ends with status 1.

The parent waits and prints `exit status N`, with the child's status.

**Example**

```
$ printf 'pear\napple\n' > in.txt
$ ./prog in.txt out.txt err.txt
exit status 0
$ cat out.txt err.txt
apple
pear
sort done
$ ./prog nope.txt out.txt err.txt
nope.txt: Error. No such file or directory
exit status 1
```
