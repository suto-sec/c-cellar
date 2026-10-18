# Statistics of any amount of numbers

Write `numstats`, which reads integers from **standard input** (separated by any white space, over any number of lines) until the input ends, and prints:

```
count: <how many>
min: <smallest>
max: <largest>
median: <median, one decimal>
```

The median is the middle value of the sorted numbers, or the mean of the two middle values when the count is even. There can be **millions** of numbers, so the array must grow as needed.

- No numbers at all: print `numstats: no numbers` to standard error and exit with status 1.
- Something that is not an integer: print `numstats: bad input` to standard error and exit with status 1 (nothing is printed on standard output).

**Example**

```
$ echo '5 1 4 2' | ./prog
count: 4
min: 1
max: 5
median: 3.0
```
