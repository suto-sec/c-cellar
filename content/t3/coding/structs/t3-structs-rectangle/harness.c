#include <stdio.h>
#include "rect.h"

int main(void)
{
    struct rect r;

    while (scanf("%d %d", &r.w, &r.h) == 2)
        printf("area=%d perimeter=%d square=%s\n", area(r), perimeter(r), is_square(r) ? "yes" : "no");
    return 0;
}
