#include <stdio.h>

int max3(int a, int b, int c);

int main(void)
{
    int a, b, c;

    while (scanf("%d %d %d", &a, &b, &c) == 3)
        printf("max = %d\n", max3(a, b, c));
    return 0;
}
