#include <stdio.h>

int main(void)
{
    int x = 10;
    int *p = &x;

    *p = 20;
    printf("x=%d %s\n", x, p == &x ? "same" : "different");
    return 0;
}
