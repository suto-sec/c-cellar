#include <stdio.h>
#include <stdlib.h>

char *my_strdup(const char *s);

int main(void)
{
    char w[128];

    while (scanf("%127s", w) == 1) {
        char *c = my_strdup(w);
        if (c == NULL)
            return 1;
        printf("copy: %s\n", c);
        free(c);
    }
    return 0;
}
