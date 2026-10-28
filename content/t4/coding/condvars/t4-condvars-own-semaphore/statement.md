# Build a semaphore

Make your own counting semaphore from a mutex, a condition variable and a counter:

```c
struct sem { /* mutex, condition, count */ };
void my_init(struct sem *s, int value);
void my_wait(struct sem *s);   /* wait until count > 0, then count-- */
void my_post(struct sem *s);   /* count++ and wake a waiter */
```

Test it: three threads each add `1` to a shared counter **100000** times, using your semaphore (initialised to `1`) as the lock. The main thread prints `counter = 300000`.

**Example**

```
$ ./prog
counter = 300000
```
