# N threads, one counter

`prog N` (with `N` from 1 to 8) creates `N` threads. Each adds `1` to a shared counter **100000** times. After joining all of them the main thread prints `counter = C`.

**Examples**

```
$ ./prog 4
counter = 400000
$ ./prog 1
counter = 100000
```
