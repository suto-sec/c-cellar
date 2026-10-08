#include <stdio.h>

typedef struct {
    int width;
    int height;
} Trect;

static Trect scale(Trect r, int k)
{
    r.width *= k;
    r.height *= k;
    return r;
}

static int area(Trect r)
{
    return r.width * r.height;
}

int main(void)
{
    Trect r;
    int k;

    while (scanf("%d %d %d", &r.width, &r.height, &k) == 3) {
        r = scale(r, k);
        printf("%d x %d, area %d\n", r.width, r.height, area(r));
    }
    return 0;
}
