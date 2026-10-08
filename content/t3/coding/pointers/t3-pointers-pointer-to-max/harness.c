#include <stdio.h>

int *find_max(int *v, int n);

int main(void)
{
    int n, v[100];

    if (scanf("%d", &n) != 1 || n < 1 || n > 100)
        return 1;
    for (int i = 0; i < n; i++)
        if (scanf("%d", &v[i]) != 1)
            return 1;
    *find_max(v, n) = 0;
    for (int i = 0; i < n; i++)
        printf("%d%s", v[i], i < n - 1 ? " " : "\n");
    return 0;
}
