#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int x;
    int *count = calloc(10, sizeof(int));

    if (count == NULL)
        return 1;
    while (scanf("%d", &x) == 1)
        if (x >= 0 && x <= 9)
            count[x]++;
    for (int d = 0; d < 10; d++)
        printf("%d: %d\n", d, count[d]);
    free(count);
    return 0;
}
