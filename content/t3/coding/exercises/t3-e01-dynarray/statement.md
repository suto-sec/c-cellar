# Sort a stream of numbers

Read integers from standard input until end of file. There is **no limit** on how many (do not use a fixed-size array).
Then print:

1. the numbers in ascending order, separated by single spaces, on one line (no trailing space);
2. a second line `count: <N>, mean: <average with 2 decimals>`.

If there are no numbers, print only `count: 0, mean: 0.00`.

```
$ echo "5 3 9 1" | ./prog
1 3 5 9
count: 4, mean: 4.50
```

Numbers may be separated by spaces and/or newlines. Free the memory you allocate.
