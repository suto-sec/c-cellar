#include <stdio.h>

int main(void)
{
    long sum = 0;
    int x, r;

    while ((r = scanf("%d", &x)) == 1)
        sum += x;
    if (r == EOF)
        printf("sum=%ld\n", sum);
    else
        printf("sum=%ld (stopped at bad input)\n", sum);
    return 0;
}
