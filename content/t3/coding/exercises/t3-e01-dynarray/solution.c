#include <stdio.h>
#include <stdlib.h>

static int cmp_int(const void *a, const void *b)
{
    int x = *(const int *) a, y = *(const int *) b;
    return (x > y) - (x < y);
}

int main(void)
{
    int *v = NULL, x;
    size_t n = 0, cap = 0;
    double sum = 0;

    while (scanf("%d", &x) == 1) {
        if (n == cap) {
            size_t ncap = cap ? cap * 2 : 8;
            int *tmp = realloc(v, ncap * sizeof(int));
            if (tmp == NULL) {
                free(v);
                return 1;
            }
            v = tmp;
            cap = ncap;
        }
        v[n++] = x;
        sum += x;
    }
    qsort(v, n, sizeof(int), cmp_int);
    for (size_t i = 0; i < n; i++)
        printf("%s%d", i ? " " : "", v[i]);
    if (n)
        printf("\n");
    printf("count: %zu, mean: %.2f\n", n, n ? sum / n : 0.0);
    free(v);
    return 0;
}
