#include <stdio.h>
#include <stdlib.h>

static int compare(const void *a, const void *b)
{
    int x = *(const int *) a, y = *(const int *) b;
    return (x > y) - (x < y);
}

int main(void)
{
    int *v = NULL, x, rc;
    size_t n = 0, cap = 0;
    double med;

    while ((rc = scanf("%d", &x)) == 1) {
        if (n == cap) {
            int *bigger;
            cap = cap ? cap * 2 : 64;
            bigger = realloc(v, cap * sizeof(int));
            if (bigger == NULL)
                return 1;
            v = bigger;
        }
        v[n++] = x;
    }
    if (rc != EOF) {
        fprintf(stderr, "numstats: bad input\n");
        return 1;
    }
    if (n == 0) {
        fprintf(stderr, "numstats: no numbers\n");
        return 1;
    }
    qsort(v, n, sizeof(int), compare);
    med = n % 2 ? v[n / 2] : ((double) v[n / 2 - 1] + v[n / 2]) / 2;
    printf("count: %zu\nmin: %d\nmax: %d\nmedian: %.1f\n", n, v[0], v[n - 1], med);
    free(v);
    return 0;
}
