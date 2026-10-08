#include <stdio.h>

int main(void)
{
    int x = 42;
    int *p = &x;

    printf("p=%p\n", (void *)p);
    printf("*p=%d\n", *p);
    return 0;
}
