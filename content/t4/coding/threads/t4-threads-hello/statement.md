# Hello from a thread

Create **one** thread. The thread prints `hello from thread` and ends. The main thread waits for it with `pthread_join` and then prints `joined`.

**Example**

```
$ ./prog
hello from thread
joined
```
