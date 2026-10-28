# Producer and consumer

`prog N` runs two threads over a **circular buffer of 4 slots**.

- The **producer** puts the numbers `1, 2, ..., N` into the buffer, one by one.
- The **consumer** takes `N` numbers out and adds them up.

Use semaphores (`empty` = free slots, `full` = filled slots, and one to protect the buffer). The main thread joins both threads and prints `sum = S`, which must be `N(N+1)/2` (`N` up to 100000, use `long`).

**Examples**

```
$ ./prog 10
sum = 55
$ ./prog 1000
sum = 500500
```
