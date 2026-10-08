#include <stdio.h>

int main(void)
{
    printf("%5s%5s%5s\n", "n", "n^2", "n^3");
    for (int n = 1; n <= 5; n++)
        printf("%5d%5d%5d\n", n, n * n, n * n * n);
    return 0;
}
