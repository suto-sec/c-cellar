#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int x, *p;

    if (scanf("%d", &x) != 1) {
        printf("bad input\n");
        return 1;
    }
    p = malloc(sizeof(int));
    if (p == NULL)
        return 1;
    *p = 2 * x;
    printf("double: %d\n", *p);
    free(p);
    return 0;
}
