#include <stdio.h>
#include "ids.h"

int main(void)
{
    int a = next_id();
    int b = next_id();
    int c = next_id();
    int d = next_id();

    printf("%d %d %d %d\n", a, b, c, d);
    return 0;
}
