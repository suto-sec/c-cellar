#include <stdlib.h>
#include <string.h>

char *join(const char *a, const char *b, char sep)
{
    size_t la = strlen(a), lb = strlen(b);
    char *r = malloc(la + 1 + lb + 1);

    if (r == NULL)
        return NULL;
    memcpy(r, a, la);
    r[la] = sep;
    memcpy(r + la + 1, b, lb);
    r[la + 1 + lb] = '\0';
    return r;
}
