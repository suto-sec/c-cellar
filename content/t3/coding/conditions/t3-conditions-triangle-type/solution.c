#include <stdio.h>

int main(void)
{
    int a, b, c, t;

    if (scanf("%d %d %d", &a, &b, &c) != 3)
        return 1;
    if (a > b) { t = a; a = b; b = t; }
    if (b > c) { t = b; b = c; c = t; }
    if (a > b) { t = a; a = b; b = t; }
    if (a <= 0 || a + b <= c)
        printf("not a triangle\n");
    else if (a == c)
        printf("equilateral\n");
    else if (a == b || b == c)
        printf("isosceles\n");
    else
        printf("scalene\n");
    return 0;
}
