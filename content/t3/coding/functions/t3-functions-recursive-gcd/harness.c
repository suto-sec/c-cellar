#include <stdio.h>

int gcd(int a, int b);

int main(void)
{
    int a, b;

    while (scanf("%d %d", &a, &b) == 2)
        printf("gcd = %d\n", gcd(a, b));
    return 0;
}
