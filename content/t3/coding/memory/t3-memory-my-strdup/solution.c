#include <stdlib.h>
#include <string.h>

char *my_strdup(const char *s)
{
    size_t n = strlen(s) + 1;
    char *copy = malloc(n);

    if (copy != NULL)
        memcpy(copy, s, n);
    return copy;
}
