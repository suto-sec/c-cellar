#include <stdio.h>

void divmod(int a, int b, int *q, int *r);

int main(void)
{
    int a, b, q, r;

    while (scanf("%d %d", &a, &b) == 2) {
        divmod(a, b, &q, &r);
        printf("q=%d r=%d\n", q, r);
    }
    return 0;
}
