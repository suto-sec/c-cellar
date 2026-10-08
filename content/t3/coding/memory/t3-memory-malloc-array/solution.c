#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int n;
    int *v;

    if (scanf("%d", &n) != 1 || n < 1)
        return 1;
    v = malloc((size_t) n * sizeof(int));
    if (v == NULL)
        return 1;
    for (int i = 0; i < n; i++)
        if (scanf("%d", &v[i]) != 1) {
            free(v);
            return 1;
        }
    for (int i = n - 1; i >= 0; i--)
        printf("%d\n", v[i]);
    free(v);
    return 0;
}
