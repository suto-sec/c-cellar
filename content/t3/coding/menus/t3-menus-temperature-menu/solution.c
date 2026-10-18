#include <stdio.h>

static float c_to_f(float c)
{
    return c * 9 / 5 + 32;
}

static float f_to_c(float f)
{
    return (f - 32) * 5 / 9;
}

int main(void)
{
    int option;
    float x;

    for (;;) {
        printf("1. Celsius to Fahrenheit\n2. Fahrenheit to Celsius\n0. Exit\nOption:\n");
        if (scanf("%d", &option) != 1)
            break;
        if (option == 0) {
            printf("Bye\n");
            break;
        } else if (option == 1) {
            printf("Celsius:\n");
            if (scanf("%f", &x) != 1)
                break;
            printf("%.1f F\n", c_to_f(x));
        } else if (option == 2) {
            printf("Fahrenheit:\n");
            if (scanf("%f", &x) != 1)
                break;
            printf("%.1f C\n", f_to_c(x));
        } else {
            printf("Invalid option\n");
        }
    }
    return 0;
}
