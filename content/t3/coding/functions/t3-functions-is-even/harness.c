#include <stdio.h>

int is_even(int n);

int main(void)
{
    int n;

    while (scanf("%d", &n) == 1)
        printf("%s\n", is_even(n) ? "yes" : "no");
    return 0;
}
