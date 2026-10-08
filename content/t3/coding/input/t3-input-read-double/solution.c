#include <stdio.h>

int main(void)
{
    double w, h;

    if (scanf("%lf %lf", &w, &h) != 2)
        return 1;
    printf("%.2f\n", w * h);
    return 0;
}
