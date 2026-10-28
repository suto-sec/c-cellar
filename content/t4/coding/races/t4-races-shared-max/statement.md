# A shared maximum

Read `N` (1 to 1000) and `N` integers. **Four threads** scan the numbers (thread `k` takes the slice `[k*N/4, (k+1)*N/4)`) and all of them update **one global** variable `best` with the largest value they have seen; the variable is protected by a mutex. The main thread prints `max: M`.

(Compared with the *four threads, four partial results* exercise, here the threads share the result instead of returning it.)

**Examples**

```
$ echo 5 3 9 -2 7 1 | ./prog
max: 9
$ echo 3 -3 -1 -2 | ./prog
max: -1
```
