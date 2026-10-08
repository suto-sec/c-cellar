#include <stdio.h>
#include <string.h>

int main(void)
{
    char w[128];

    if (scanf("%127s", w) != 1)
        return 1;
    printf("%zu\n", strlen(w));
    return 0;
}
