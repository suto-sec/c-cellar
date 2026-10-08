#include <stdio.h>

int main(void)
{
    int a = 3, b = 7, tmp;

    tmp = a;
    a = b;
    b = tmp;
    printf("a=%d b=%d\n", a, b);
    return 0;
}
