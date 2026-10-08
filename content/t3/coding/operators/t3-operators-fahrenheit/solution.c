#include <stdio.h>

int main(void)
{
    int f;

    if (scanf("%d", &f) != 1)
        return 1;
    printf("%.1f\n", (f - 32) * 5.0 / 9);
    return 0;
}
