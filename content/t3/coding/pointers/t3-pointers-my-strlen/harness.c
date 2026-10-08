#include <stdio.h>
#include <stddef.h>

size_t my_strlen(const char *s);

int main(void)
{
    char w[128];

    while (scanf("%127s", w) == 1)
        printf("%s: %zu\n", w, my_strlen(w));
    return 0;
}
