#include <stdbool.h>
#include <stdio.h>

static bool is_square(int n)
{
    long r = 0;

    while (r * r < n)
        r++;
    return r * r == n;
}

int main(void)
{
    int n;

    while (scanf("%d", &n) == 1)
        printf("%d %s\n", n, is_square(n) ? "yes" : "no");
    return 0;
}
