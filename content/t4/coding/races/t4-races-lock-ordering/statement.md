# Two locks, no deadlock

Two accounts, `acc[0]` and `acc[1]`, each start with `1000` and each has **its own mutex**.

- Thread 1 moves 1 unit from account 0 to account 1, **5000 times**.
- Thread 2 moves 1 unit from account 1 to account 0, **5000 times**.

A transfer must lock **both** accounts (so that nobody sees the money in flight). Write it so that the program can never deadlock. The main thread joins the threads and prints `a=A b=B total=T`.

**Example**

```
$ ./prog
a=1000 b=1000 total=2000
```
