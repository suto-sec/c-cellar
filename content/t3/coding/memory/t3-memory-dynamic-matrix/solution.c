#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int r, c;
    int **m;

    if (scanf("%d %d", &r, &c) != 2 || r < 1 || c < 1)
        return 1;
    m = malloc((size_t) r * sizeof(int *));
    if (m == NULL)
        return 1;
    for (int i = 0; i < r; i++) {
        m[i] = malloc((size_t) c * sizeof(int));
        if (m[i] == NULL)
            return 1;
        for (int j = 0; j < c; j++)
            m[i][j] = i * c + j;
    }
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++)
            printf("%d%s", m[i][j], j < c - 1 ? " " : "\n");
        free(m[i]);
    }
    free(m);
    return 0;
}
