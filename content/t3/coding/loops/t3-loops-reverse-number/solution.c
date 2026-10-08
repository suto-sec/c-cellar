#include <stdio.h>

int main(void)
{
    int n;
    long rev = 0;

    if (scanf("%d", &n) != 1)
        return 1;
    while (n > 0) {
        rev = rev * 10 + n % 10;
        n /= 10;
    }
    printf("%ld\n", rev);
    return 0;
}
