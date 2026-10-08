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
    int best = 0;

    for (int i = 1; i < n; i++)
        if (v[i] > v[best])
            best = i;
    printf("max=%d at %d\n", v[best], best);
    return 0;
}
