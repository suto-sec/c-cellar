#include <stdio.h>

int main(void)
{
    long n;
    int prime = 1;

    if (scanf("%ld", &n) != 1)
        return 1;
    if (n < 2)
        prime = 0;
    for (long d = 2; d * d <= n && prime; d++)
        if (n % d == 0)
            prime = 0;
    printf("%s\n", prime ? "prime" : "not prime");
    return 0;
}
