#include <stdio.h>
#include <ctype.h>

int main(void)
{
    char line[1024];
    int count[26] = {0};

    if (fgets(line, sizeof(line), stdin) == NULL)
        return 1;
    for (char *p = line; *p; p++)
        if (isalpha((unsigned char) *p))
            count[tolower((unsigned char) *p) - 'a']++;
    for (int i = 0; i < 26; i++)
        if (count[i] > 0)
            printf("%c: %d\n", 'a' + i, count[i]);
    return 0;
}
