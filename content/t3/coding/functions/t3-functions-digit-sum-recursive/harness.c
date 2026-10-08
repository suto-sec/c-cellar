#include <stdio.h>

int digit_sum(int n);

int main(void)
{
    int n;

    while (scanf("%d", &n) == 1)
        printf("digit_sum(%d) = %d\n", n, digit_sum(n));
    return 0;
}
