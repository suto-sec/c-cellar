# Everybody appends to the list

`prog N` (with `N` from 1 to 8) creates `N` threads that share a global array `list[8000]` and a counter `count` of its used entries. Thread number `i` (from 0) appends the value `i + 1` to the list **1000 times** (`list[count] = value; count++;`).

After joining them all, the main thread prints `entries: E` (the final `count`) and `sum: S` (the sum of all the values in the list).

**Examples**

```
$ ./prog 3
entries: 3000
sum: 6000
$ ./prog 1
entries: 1000
sum: 1000
```
