#include <stdio.h>

int main(void)
{
    int r, c, m[10][10];

    if (scanf("%d %d", &r, &c) != 2 || r < 1 || c < 1 || r > 10 || c > 10)
        return 1;
    for (int i = 0; i < r; i++)
        for (int j = 0; j < c; j++)
            if (scanf("%d", &m[i][j]) != 1)
                return 1;
    printf("rows:");
    for (int i = 0; i < r; i++) {
        int s = 0;
        for (int j = 0; j < c; j++)
            s += m[i][j];
        printf(" %d", s);
    }
    printf("\ncols:");
    for (int j = 0; j < c; j++) {
        int s = 0;
        for (int i = 0; i < r; i++)
            s += m[i][j];
        printf(" %d", s);
    }
    printf("\n");
    return 0;
}
