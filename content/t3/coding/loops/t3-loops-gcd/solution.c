#include <stdio.h>

int main(void)
{
    int a, b, t;

    if (scanf("%d %d", &a, &b) != 2)
        return 1;
    while (b != 0) {
        t = a % b;
        a = b;
        b = t;
    }
    printf("%d\n", a);
    return 0;
}
