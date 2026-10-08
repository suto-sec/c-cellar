#include <stdio.h>

int main(void)
{
    char line[256];

    if (fgets(line, sizeof(line), stdin) == NULL)
        return 1;
    printf(">> %s", line);
    return 0;
}
