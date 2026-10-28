# Wait for the salary

A bank thread and an employee thread share a global `balance`, which starts at `0`.

- The **bank** deposits `3000` **twice**: it sleeps one second (`sleep(1)`, without holding the mutex), then deposits and wakes the employee.
- The **employee** withdraws `1000` **six times**. If the balance is below `1000` it must **wait** on a condition variable (releasing the mutex) until a deposit arrives.

The main thread joins both and prints `withdrew 6000` and `balance 0` (two lines). The program takes about two seconds.

**Example**

```
$ ./prog
withdrew 6000
balance 0
```
