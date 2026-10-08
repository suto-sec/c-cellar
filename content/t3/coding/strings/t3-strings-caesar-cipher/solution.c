#include <stdio.h>
#include <ctype.h>

int main(void)
{
    int k;
    char line[1024];

    if (scanf("%d ", &k) != 1)
        return 1;
    if (fgets(line, sizeof(line), stdin) == NULL)
        return 1;
    for (char *p = line; *p; p++) {
        if (isupper((unsigned char) *p))
            *p = 'A' + (*p - 'A' + k) % 26;
        else if (islower((unsigned char) *p))
            *p = 'a' + (*p - 'a' + k) % 26;
    }
    printf("%s", line);
    return 0;
}
