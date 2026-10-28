# Moving money both ways

Two global accounts start with `a = 1000` and `b = 1000`.

- Thread 1 moves 1 unit from `a` to `b` (`a--; b++;`) **10000 times**.
- Thread 2 moves 1 unit from `b` to `a` **10000 times**.

Protect the transfers with a mutex. The main thread joins both threads and prints `a=A b=B total=T`. Both balances end where they started, and the total is always `2000`.

**Example**

```
$ ./prog
a=1000 b=1000 total=2000
```
