#include <stdio.h>

#define MAX 100

int main(void)
{
    int n, v[MAX];

    if (scanf("%d", &n) != 1 || n < 1 || n > MAX)
        return 1;
    for (int i = 0; i < n; i++)
        if (scanf("%d", &v[i]) != 1)
            return 1;
    int k;

    if (scanf("%d", &k) != 1)
        return 1;
    k %= n;
    for (int i = 0; i < n; i++)
        printf("%d%s", v[(i + k) % n], i < n - 1 ? " " : "\n");
    return 0;
}
