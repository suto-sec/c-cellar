#include "point.h"

struct point add(struct point a, struct point b)
{
    struct point r = {a.x + b.x, a.y + b.y};
    return r;
}

void scale(struct point *p, int k)
{
    p->x *= k;
    p->y *= k;
}

static int absval(int v) { return v < 0 ? -v : v; }

int manhattan(struct point a, struct point b)
{
    return absval(a.x - b.x) + absval(a.y - b.y);
}
