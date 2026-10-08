#include <stdio.h>
#include <stdlib.h>

char *join(const char *a, const char *b, char sep);

int main(void)
{
    char a[128], b[128];

    while (scanf("%127s %127s", a, b) == 2) {
        char *j = join(a, b, '-');
        if (j == NULL)
            return 1;
        printf("%s\n", j);
        free(j);
    }
    return 0;
}
