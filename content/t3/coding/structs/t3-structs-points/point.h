#ifndef POINT_H
#define POINT_H

struct point { int x, y; };

struct point add(struct point a, struct point b);
void scale(struct point *p, int k);
int manhattan(struct point a, struct point b);

#endif
