#include <stdio.h>
#include <ctype.h>

int main(void)
{
    char line[1024];
    int words = 0, inword = 0;

    if (fgets(line, sizeof(line), stdin) == NULL)
        return 1;
    for (char *p = line; *p; p++) {
        if (isspace((unsigned char) *p))
            inword = 0;
        else if (!inword) {
            inword = 1;
            words++;
        }
    }
    printf("%d\n", words);
    return 0;
}
