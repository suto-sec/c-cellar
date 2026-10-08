#include <stdio.h>
#include <ctype.h>

int main(void)
{
    char line[1024];

    if (fgets(line, sizeof(line), stdin) == NULL)
        return 1;
    for (char *p = line; *p; p++)
        *p = toupper((unsigned char) *p);
    printf("%s", line);
    return 0;
}
