# Two threads, one counter

Two threads each add `1` to a shared global counter **500000** times. The main thread joins them and prints `counter = 1000000`.

Without protection the program prints a smaller number almost every time (lost updates): use a **mutex** so that it always prints `1000000`.

**Example**

```
$ ./prog
counter = 1000000
```
