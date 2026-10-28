# A thread that returns a result

`prog N` creates a thread that receives `N`, computes `N * N` and **returns** it to the main thread (through `pthread_join`, not through a global). The main thread prints `square: S`.

**Examples**

```
$ ./prog 12
square: 144
$ ./prog -3
square: 9
```
