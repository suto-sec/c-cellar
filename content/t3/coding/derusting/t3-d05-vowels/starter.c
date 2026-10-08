#include <stdio.h>

int main(void)
{
    char line[1024];

    if (fgets(line, sizeof(line), stdin) == NULL)
        return 1;
    /* TODO: count characters (without the newline) and vowels */
    return 0;
}
