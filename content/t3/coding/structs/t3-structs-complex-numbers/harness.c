#include <stdio.h>
#include "complex.h"

int main(void)
{
    struct complex a, b, s, p;

    while (scanf("%lf %lf %lf %lf", &a.re, &a.im, &b.re, &b.im) == 4) {
        s = add(a, b);
        p = mul(a, b);
        printf("sum=(%.2f, %.2f) product=(%.2f, %.2f)\n", s.re, s.im, p.re, p.im);
    }
    return 0;
}
