#include <stdio.h>

long fact(int n);

int main(void)
{
    int n;

    while (scanf("%d", &n) == 1)
        printf("%d! = %ld\n", n, fact(n));
    return 0;
}
