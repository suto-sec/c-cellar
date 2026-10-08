#include <stdio.h>

int is_prime(int n);

int main(void)
{
    int n;

    while (scanf("%d", &n) == 1)
        printf("%s\n", is_prime(n) ? "prime" : "not prime");
    return 0;
}
