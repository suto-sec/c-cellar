# Write, then read back

`./prog FILE N` creates `FILE` (emptying it if it exists) and writes the squares `1`, `4`, `9`, ... of the numbers 1 to `N` (`N` from 1 to 100), one per line. Then it reads them back **from the same open stream** and prints `sum = <their sum>`. Open the file once, with the mode `"w+"`.

**Example**

```
$ ./prog squares.txt 4
sum = 30
$ cat squares.txt
1
4
9
16
```
