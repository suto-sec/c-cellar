# A, B, C, A, B, C

`prog N` (with `N` from 1 to 10) creates three threads, A, B and C. Each prints its own letter `N` times, **without a newline**, and they must print in strict rotation: `A`, `B`, `C`, `A`, `B`, `C`, ... Use one mutex and condition variable(s).

After joining the threads the main thread prints a newline.

**Examples**

```
$ ./prog 2
ABCABC
$ ./prog 4
ABCABCABCABC
```
