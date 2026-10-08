# Points with structs

The header `point.h` (already in your folder) defines:

```c
struct point { int x, y; };
```

Implement in `answer.c`:

```c
struct point add(struct point a, struct point b);   /* component-wise sum, returned by value */
void scale(struct point *p, int k);                 /* multiply both components of *p by k, in place */
int manhattan(struct point a, struct point b);      /* |a.x-b.x| + |a.y-b.y| */
```

The provided `main` reads `x1 y1 x2 y2 k` and prints the sum scaled by `k` and the Manhattan distance:

```
$ echo "1 2  3 -4  2" | ./prog
sum=(8, -4) dist=8
```
