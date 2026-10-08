#include <stdio.h>

int main(void)
{
    int n;
    long f = 1;

    if (scanf("%d", &n) != 1)
        return 1;
    for (int i = 2; i <= n; i++)
        f *= i;
    printf("%d! = %ld\n", n, f);
    return 0;
}
