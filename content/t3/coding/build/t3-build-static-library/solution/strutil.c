#include "strutil.h"

int count_char(const char *s, char c)
{
    int n = 0;

    for (; *s; s++)
        if (*s == c)
            n++;
    return n;
}
