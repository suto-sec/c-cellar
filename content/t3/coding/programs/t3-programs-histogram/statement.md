# A histogram of numbers

`histogram FILE` reads integers from the file (separated by whitespace). Values from 0 to 99 are counted in buckets of ten (`00-09`, `10-19`, ... `90-99`); other values are ignored. Print one line per bucket **that has at least one value**: the bucket label, a colon, a space and one `*` per value (`10-19: ***`). Missing file: `cannot open FILE` on standard error, exit 1.

**Example**

```
$ ./prog f.txt
00-09: **
10-19: **
50-59: ***
90-99: *
```
