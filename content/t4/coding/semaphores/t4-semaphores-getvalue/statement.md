# Watch a semaphore count

Create a semaphore with initial value `2` and print its value after each step (`sem_getvalue`):

```
initial: 2
after wait: 1
after second wait: 0
after post: 1
```

(`wait` twice, then `post` once; no threads needed. The second `wait` does not block because the count was still 1.) Destroy the semaphore at the end.
