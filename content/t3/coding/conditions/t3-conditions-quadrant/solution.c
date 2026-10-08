#include <stdio.h>

int main(void)
{
    int x, y;

    if (scanf("%d %d", &x, &y) != 2)
        return 1;
    if (x == 0 && y == 0)
        printf("origin\n");
    else if (x == 0 || y == 0)
        printf("axis\n");
    else if (x > 0 && y > 0)
        printf("Q1\n");
    else if (x < 0 && y > 0)
        printf("Q2\n");
    else if (x < 0 && y < 0)
        printf("Q3\n");
    else
        printf("Q4\n");
    return 0;
}
