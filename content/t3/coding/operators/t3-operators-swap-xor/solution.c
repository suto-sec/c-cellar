#include <stdio.h>

int main(void)
{
    int a, b;

    if (scanf("%d %d", &a, &b) != 2)
        return 1;
    a = a ^ b;
    b = a ^ b;
    a = a ^ b;
    printf("%d %d\n", a, b);
    return 0;
}
