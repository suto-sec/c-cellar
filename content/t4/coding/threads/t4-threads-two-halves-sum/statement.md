# Two threads, two halves

Read `N` (at most 1000) and then `N` integers from standard input. Split the numbers into two halves, the first `N / 2` numbers and the rest. **Two threads**, one per half, add up their half. The main thread adds the two partial sums and prints `total: T`.

**Examples**

```
$ echo 6 1 2 3 4 5 6 | ./prog
total: 21
$ echo 0 | ./prog
total: 0
```
