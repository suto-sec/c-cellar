#include <stdio.h>

void print_stars(int n);

int main(void)
{
    int n;

    while (scanf("%d", &n) == 1)
        print_stars(n);
    return 0;
}
