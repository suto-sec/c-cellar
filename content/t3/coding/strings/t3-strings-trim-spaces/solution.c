#include <string.h>

void trim(char *s)
{
    size_t start = 0, len = strlen(s);

    while (len > 0 && s[len - 1] == ' ')
        len--;
    s[len] = '\0';
    while (s[start] == ' ')
        start++;
    memmove(s, s + start, len - start + 1);
}
