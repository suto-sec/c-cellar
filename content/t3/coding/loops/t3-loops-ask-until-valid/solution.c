#include <stdio.h>

int main(void)
{
    int n;

    do {
        if (scanf("%d", &n) != 1)
            return 1;
        if (n < 1 || n > 10)
            puts("try again");
    } while (n < 1 || n > 10);
    printf("accepted %d\n", n);
    return 0;
}
