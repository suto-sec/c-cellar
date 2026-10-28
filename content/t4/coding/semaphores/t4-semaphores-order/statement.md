# First this, then that

The main thread creates two threads: thread B (creates first) prints `second`, thread A (created after it) prints `first`. Use **one semaphore** so that `first` is **always** printed before `second`, whichever thread starts running first. The main thread joins both.

**Example**

```
$ ./prog
first
second
```
