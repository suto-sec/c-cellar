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
    for (int j = 0; j < c; j++)
        for (int i = 0; i < r; i++)
            printf("%d%s", m[i][j], i < r - 1 ? " " : "\n");
    return 0;
}
