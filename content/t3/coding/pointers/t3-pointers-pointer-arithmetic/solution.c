#include <stdio.h>

int main(void)
{
    int a[] = {10, 20, 30, 40, 50};
    int *p = a;

    printf("%d %d %ld\n", *(p + 2), p[3], (long) (&a[4] - &a[0]));
    return 0;
}
