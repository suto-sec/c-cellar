#include <stdio.h>

long power(int base, int exp);

int main(void)
{
    int b, e;

    while (scanf("%d %d", &b, &e) == 2)
        printf("%d^%d = %ld\n", b, e, power(b, e));
    return 0;
}
