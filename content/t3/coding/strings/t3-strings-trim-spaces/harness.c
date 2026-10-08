#include <stdio.h>
#include <string.h>

void trim(char *s);

int main(void)
{
    char line[256];

    if (fgets(line, sizeof(line), stdin) == NULL)
        return 1;
    line[strcspn(line, "\n")] = '\0';
    trim(line);
    printf("[%s]\n", line);
    return 0;
}
