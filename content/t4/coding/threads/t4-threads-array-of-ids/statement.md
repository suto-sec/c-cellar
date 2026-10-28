# Each thread gets its own number

`prog N` (with `N` from 1 to 8) creates `N` threads at once. Thread number `i` (from 0) computes `i * i` and stores it in `results[i]`. After joining all of them, the main thread prints one line per thread, `i: i*i`.

**Example**

```
$ ./prog 4
0: 0
1: 1
2: 4
3: 9
```
