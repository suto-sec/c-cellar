#include <stdio.h>

#define PI 3.14159265f

static float triangle(float base, float height)
{
    return base * height / 2;
}

static float rectangle(float width, float height)
{
    return width * height;
}

static float square(float side)
{
    return side * side;
}

static float circle(float radius)
{
    return PI * radius * radius;
}

int main(void)
{
    int option;
    float a, b;

    for (;;) {
        printf("1. Triangle area\n2. Rectangle area\n3. Square area\n4. Circle area\n5. Exit\nOption:\n");
        if (scanf("%d", &option) != 1)
            break;
        if (option == 5) {
            printf("Bye\n");
            break;
        }
        switch (option) {
        case 1:
            printf("Base:\n");
            if (scanf("%f", &a) != 1) return 0;
            printf("Height:\n");
            if (scanf("%f", &b) != 1) return 0;
            printf("Area = %.2f\n", triangle(a, b));
            break;
        case 2:
            printf("Width:\n");
            if (scanf("%f", &a) != 1) return 0;
            printf("Height:\n");
            if (scanf("%f", &b) != 1) return 0;
            printf("Area = %.2f\n", rectangle(a, b));
            break;
        case 3:
            printf("Side:\n");
            if (scanf("%f", &a) != 1) return 0;
            printf("Area = %.2f\n", square(a));
            break;
        case 4:
            printf("Radius:\n");
            if (scanf("%f", &a) != 1) return 0;
            printf("Area = %.2f\n", circle(a));
            break;
        default:
            printf("Invalid option\n");
        }
    }
    return 0;
}
