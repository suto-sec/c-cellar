#include <stdio.h>

int main(void)
{
    double alpha, tau, t;
    int n, i;

    if (scanf("%lf %lf", &alpha, &tau) != 2 || scanf("%d", &n) != 1)
        return 1;
    for (i = 1; i <= n; i++) {
        if (scanf("%lf", &t) != 1)
            return 1;
        printf("%d: %.2f\n", i, tau);
        tau = alpha * t + (1 - alpha) * tau;
    }
    printf("next: %.2f\n", tau);
    return 0;
}
