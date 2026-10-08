#include "rect.h"

int area(struct rect r)
{
    return r.w * r.h;
}

int perimeter(struct rect r)
{
    return 2 * (r.w + r.h);
}

int is_square(struct rect r)
{
    return r.w == r.h;
}
