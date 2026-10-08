#include <stdio.h>

static void row(int n, int stars)
{
    for (int i = 0; i < (n - stars) / 2; i++)
        putchar(' ');
    for (int i = 0; i < stars; i++)
        putchar('*');
    putchar('\n');
}

int main(void)
{
    int n;

    if (scanf("%d", &n) != 1)
        return 1;
    for (int s = 1; s <= n; s += 2)
        row(n, s);
    for (int s = n - 2; s >= 1; s -= 2)
        row(n, s);
    return 0;
}
