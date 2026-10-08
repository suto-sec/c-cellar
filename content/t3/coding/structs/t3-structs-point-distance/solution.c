#include <stdio.h>
#include <math.h>

struct point {
    double x, y;
};

int main(void)
{
    struct point a, b;

    if (scanf("%lf %lf %lf %lf", &a.x, &a.y, &b.x, &b.y) != 4)
        return 1;
    printf("%.2f\n", sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y)));
    return 0;
}
