# Pointer arithmetic

Declare `int a[] = {10, 20, 30, 40, 50};` and `int *p = a;`. Print on one line, in this order and separated by spaces: the value `*(p + 2)`, the value `p[3]`, and the distance between `&a[4]` and `&a[0]` (a pointer difference, as a number of elements).

**Example**

```
$ ./prog
30 40 4
```
