#include <stdio.h>

int main(void)
{
    int n;

    if (scanf("%d", &n) != 1 || n < 1)
        return 1;
    int a[n];
    int max;

    for (int i = 0; i < n; i++) {
        if (scanf("%d", &a[i]) != 1)
            return 1;
        if (i == 0 || a[i] > max)
            max = a[i];
    }
    for (int i = 0; i < n; i++)
        printf("%d%c", max - a[i], i < n - 1 ? ' ' : '\n');
    return 0;
}
