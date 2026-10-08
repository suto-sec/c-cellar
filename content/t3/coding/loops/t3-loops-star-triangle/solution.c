#include <stdio.h>

int main(void)
{
    int n;

    if (scanf("%d", &n) != 1)
        return 1;
    for (int row = 1; row <= n; row++) {
        for (int i = 0; i < row; i++)
            putchar('*');
        putchar('\n');
    }
    return 0;
}
