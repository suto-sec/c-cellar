#include <stdio.h>

#define PI 3.14159

int main(void)
{
    double r = 3;

    printf("area=%.2f\n", PI * r * r);
    printf("circumference=%.2f\n", 2 * PI * r);
    return 0;
}
