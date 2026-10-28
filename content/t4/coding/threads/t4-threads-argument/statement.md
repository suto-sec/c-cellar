# Give the thread an argument

`prog N` creates a thread and hands it the number `N` (as an `int`, through the `void *` argument). The thread prints `thread got N`. The main thread joins it.

**Example**

```
$ ./prog 42
thread got 42
```
