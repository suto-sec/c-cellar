#include <stdio.h>
#include <stdlib.h>

static long sum(const int *v, int n)
{
    long s = 0;
    for (int i = 0; i < n; i++)
        s += v[i];
    return s;
}

static int largest(const int *v, int n)
{
    int m = v[0];
    for (int i = 1; i < n; i++)
        if (v[i] > m)
            m = v[i];
    return m;
}

static int smallest(const int *v, int n)
{
    int m = v[0];
    for (int i = 1; i < n; i++)
        if (v[i] < m)
            m = v[i];
    return m;
}

static void print_sorted(const int *v, int n)
{
    int *c = malloc((size_t) n * sizeof(int));
    if (c == NULL)
        return;
    for (int i = 0; i < n; i++)
        c[i] = v[i];
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - 1 - i; j++)
            if (c[j] > c[j + 1]) {
                int t = c[j];
                c[j] = c[j + 1];
                c[j + 1] = t;
            }
    printf("sorted:");
    for (int i = 0; i < n; i++)
        printf(" %d", c[i]);
    printf("\n");
    free(c);
}

int main(void)
{
    int n, option;
    int *v;

    if (scanf("%d", &n) != 1 || n <= 0)
        return 1;
    v = malloc((size_t) n * sizeof(int));
    if (v == NULL)
        return 1;
    for (int i = 0; i < n; i++)
        if (scanf("%d", &v[i]) != 1)
            return 1;
    while (scanf("%d", &option) == 1 && option != 0) {
        switch (option) {
        case 1: printf("sum = %ld\n", sum(v, n)); break;
        case 2: printf("average = %.2f\n", (double) sum(v, n) / n); break;
        case 3: printf("max = %d\n", largest(v, n)); break;
        case 4: printf("min = %d\n", smallest(v, n)); break;
        case 5: print_sorted(v, n); break;
        default: printf("invalid option\n");
        }
    }
    free(v);
    return 0;
}
