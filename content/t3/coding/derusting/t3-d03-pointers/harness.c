#include <stdio.h>

void swap(int *a, int *b);
void minmax(const int *v, int n, int *min, int *max);

int main(void)
{
    int v[100], n, lo = 0, hi = 0;

    if (scanf("%d", &n) != 1 || n < 1 || n > 100)
        return 1;
    for (int i = 0; i < n; i++)
        if (scanf("%d", &v[i]) != 1)
            return 1;
    minmax(v, n, &lo, &hi);
    printf("min=%d max=%d\n", lo, hi);
    swap(&lo, &hi);
    printf("swapped: %d %d\n", lo, hi);
    return 0;
}
