#include <stdio.h>

int square(int x);

int main(void)
{
    int x;

    while (scanf("%d", &x) == 1)
        printf("square(%d) = %d\n", x, square(x));
    return 0;
}
