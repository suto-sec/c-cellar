# Binary file of integers

`binsum N1 N2 ...` writes all its integer arguments to the binary file `numbers.bin` with a single `fwrite` (as `int` values), closes it, opens it again, reads them back with `fread` and prints `count=<how many> sum=<their sum>`. With no arguments it prints `count=0 sum=0` (and creates an empty file).

**Example**

```
$ ./prog 10 20 30
count=3 sum=60
```
