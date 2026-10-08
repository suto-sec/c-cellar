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
    for (int i = n - 1; i >= 0; i--)
        printf("%d%s", v[i], i ? " " : "\n");
    return 0;
}
