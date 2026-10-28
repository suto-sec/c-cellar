# Copies, not sharing

A forked child gets a **copy** of the parent's memory. Show it:

1. The program has an `int x` that starts at `10`.
2. After `fork`, the child sets `x` to `20` and prints `child: x = 20`.
3. The parent waits for the child, then prints its own `x` as `parent: x = 10`.

**Example**

```
$ ./prog
child: x = 20
parent: x = 10
```
