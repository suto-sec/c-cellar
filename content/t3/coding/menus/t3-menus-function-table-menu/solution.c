#include <stdio.h>

static double add(double a, double b) { return a + b; }
static double sub(double a, double b) { return a - b; }
static double mul(double a, double b) { return a * b; }
static double divide(double a, double b) { return a / b; }

int main(void)
{
    double (*op[4])(double, double) = { add, sub, mul, divide };
    int option;
    double a, b;

    while (scanf("%d", &option) == 1 && option != 0) {
        if (scanf("%lf %lf", &a, &b) != 2)
            break;
        if (option < 1 || option > 4)
            printf("invalid option\n");
        else if (option == 4 && b == 0)
            printf("division by zero\n");
        else
            printf("%.2f\n", op[option - 1](a, b));
    }
    return 0;
}
