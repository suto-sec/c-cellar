#include <stdio.h>
#include "util.h"

int main(void)
{
    int v, lo, hi;

    if (scanf("%d %d %d", &v, &lo, &hi) != 3)
        return 1;
    printf("%d\n", clamp(v, lo, hi));
    return 0;
}
