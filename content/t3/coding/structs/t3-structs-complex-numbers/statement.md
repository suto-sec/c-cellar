# Complex numbers

The header `complex.h` (in your folder) defines `struct complex { double re, im; };` and declares two functions. Write them in `answer.c` (start with `#include "complex.h"`):

```c
struct complex add(struct complex a, struct complex b);   /* (a.re + b.re) + (a.im + b.im)i */
struct complex mul(struct complex a, struct complex b);   /* (a.re*b.re - a.im*b.im) + (a.re*b.im + a.im*b.re)i */
```

A hidden `main` reads `re1 im1 re2 im2` and prints the sum and the product with two decimals.

**Example**

Input:
```
1 2 3 4
```
Output:
```
sum=(4.00, 6.00) product=(-5.00, 10.00)
```
