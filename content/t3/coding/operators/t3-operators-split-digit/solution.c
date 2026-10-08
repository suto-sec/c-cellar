#include <stdio.h>

int main(void)
{
    int n;

    if (scanf("%d", &n) != 1)
        return 1;
    printf("last=%d rest=%d\n", n % 10, n / 10);
    return 0;
}
