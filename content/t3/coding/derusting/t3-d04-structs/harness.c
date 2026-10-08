#include <stdio.h>
#include "point.h"

int main(void)
{
    struct point a, b;
    int k;

    if (scanf("%d %d %d %d %d", &a.x, &a.y, &b.x, &b.y, &k) != 5)
        return 1;
    struct point s = add(a, b);
    scale(&s, k);
    printf("sum=(%d, %d) dist=%d\n", s.x, s.y, manhattan(a, b));
    return 0;
}
