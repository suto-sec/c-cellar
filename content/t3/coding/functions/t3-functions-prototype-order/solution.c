#include <stdio.h>

double average(int a, int b);

int main(void)
{
    int a, b;

    if (scanf("%d %d", &a, &b) != 2)
        return 1;
    printf("%.1f\n", average(a, b));
    return 0;
}

double average(int a, int b)
{
    return (a + b) / 2.0;
}
