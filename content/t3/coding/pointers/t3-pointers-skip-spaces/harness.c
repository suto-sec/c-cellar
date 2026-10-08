#include <stdio.h>
#include <string.h>

void skip_spaces(char **p);

int main(void)
{
    char line[256], *p = line;

    if (fgets(line, sizeof(line), stdin) == NULL)
        return 1;
    line[strcspn(line, "\n")] = '\0';
    skip_spaces(&p);
    printf("[%s]\n", p);
    return 0;
}
