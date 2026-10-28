# Collect every child

`prog N` creates `N` children (`N` from 0 to 10). Child number `i` (counting from 1) ends with `exit(i)`. They are all created first, without waiting in between.

The parent then collects them all with a loop of `wait` and prints the **sum of their exit statuses**, as `sum of statuses: S`.

**Examples**

```
$ ./prog 4
sum of statuses: 10
$ ./prog 0
sum of statuses: 0
```
