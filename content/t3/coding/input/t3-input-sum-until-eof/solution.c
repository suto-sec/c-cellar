#include <stdio.h>

int main(void)
{
    long sum = 0;
    int x;

    while (scanf("%d", &x) == 1)
        sum += x;
    printf("%ld\n", sum);
    return 0;
}
