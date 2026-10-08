#include <stdio.h>

int main(void)
{
    long n;
    int steps = 0;

    if (scanf("%ld", &n) != 1)
        return 1;
    while (n != 1) {
        n = (n % 2 == 0) ? n / 2 : 3 * n + 1;
        steps++;
    }
    printf("%d\n", steps);
    return 0;
}
