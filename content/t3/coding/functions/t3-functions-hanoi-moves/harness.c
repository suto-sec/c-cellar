#include <stdio.h>

long hanoi(int n);

int main(void)
{
    int n;

    while (scanf("%d", &n) == 1)
        printf("hanoi(%d) = %ld\n", n, hanoi(n));
    return 0;
}
