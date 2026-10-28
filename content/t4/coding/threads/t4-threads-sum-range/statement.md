# Add up 1 to N in a thread

`prog N` starts one thread that adds up the numbers from `1` to `N` and leaves the result for the main thread (both the limit and the result travel in one **struct** whose address is the thread's argument). The main thread prints `sum = S`.

**Examples**

```
$ ./prog 10
sum = 55
$ ./prog 0
sum = 0
```
