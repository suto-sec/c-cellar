# Two passes over a file

`./prog FILE` reads the integers of `FILE` (one per line, at least one). Print `average = <average with 2 decimals>`, then, one per line and in file order, the numbers that are **greater than** the average. Do not store the numbers: read the file twice, going back to the beginning with `rewind`.

**Example**

```
$ cat nums.txt
4
8
15
$ ./prog nums.txt
average = 9.00
15
```
