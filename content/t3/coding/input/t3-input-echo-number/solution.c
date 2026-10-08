#include <stdio.h>

int main(void)
{
    int n;

    if (scanf("%d", &n) != 1)
        return 1;
    printf("You typed: %d\n", n);
    return 0;
}
