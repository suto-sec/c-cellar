# Functions on a struct

The header `rect.h` (already in your folder) defines `struct rect { int w, h; };` and declares three functions. Write them in `answer.c` (start it with `#include "rect.h"`):

```c
int area(struct rect r);        /* w * h */
int perimeter(struct rect r);   /* 2 * (w + h) */
int is_square(struct rect r);   /* 1 if w == h, otherwise 0 */
```

A hidden `main` reads `w h` and prints `area=.. perimeter=.. square=yes|no`.

**Example**

Input:
```
3 4
```
Output:
```
area=12 perimeter=14 square=no
```
