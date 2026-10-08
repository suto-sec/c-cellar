#include <stdio.h>

int main(void)
{
    int x, pos = 0, neg = 0;

    while (scanf("%d", &x) == 1 && x != 0) {
        if (x > 0)
            pos++;
        else
            neg++;
    }
    printf("positive=%d negative=%d\n", pos, neg);
    return 0;
}
