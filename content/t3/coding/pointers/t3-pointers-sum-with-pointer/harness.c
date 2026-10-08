#include <stdio.h>

int sum(const int *v, int n);

int main(void)
{
    int n, v[100];

    if (scanf("%d", &n) != 1 || n < 1 || n > 100)
        return 1;
    for (int i = 0; i < n; i++)
        if (scanf("%d", &v[i]) != 1)
            return 1;
    printf("sum=%d\n", sum(v, n));
    return 0;
}
