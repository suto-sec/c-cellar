#include <stdio.h>

void my_strcpy(char *dst, const char *src);

int main(void)
{
    char w[128], copy[128];

    while (scanf("%127s", w) == 1) {
        my_strcpy(copy, w);
        printf("copy: %s\n", copy);
    }
    return 0;
}
