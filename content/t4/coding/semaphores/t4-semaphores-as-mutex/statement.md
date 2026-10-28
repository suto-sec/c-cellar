# A semaphore as a lock

Two threads each add `1` to a shared counter **500000** times. Protect the counter with a **semaphore** initialised to `1` (not a mutex). The main thread prints `counter = 1000000`.

**Example**

```
$ ./prog
counter = 1000000
```
