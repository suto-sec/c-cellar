#include <stdio.h>

int fib(int n);

int main(void)
{
    int n;

    while (scanf("%d", &n) == 1)
        printf("fib(%d) = %d\n", n, fib(n));
    return 0;
}
