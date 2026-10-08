#include <stdio.h>

int main(void)
{
    int n;

    if (scanf("%d", &n) != 1)
        return 1;
    for (int i = 1; i <= 10; i++)
        printf("%d x %d = %d\n", n, i, n * i);
    return 0;
}
