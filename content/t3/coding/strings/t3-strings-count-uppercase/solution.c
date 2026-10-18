#include <stdio.h>
#include <ctype.h>

int main(void)
{
    char line[1000];
    int count = 0;

    if (fgets(line, sizeof(line), stdin) != NULL)
        for (int i = 0; line[i] != '\0'; i++)
            if (isupper((unsigned char) line[i]))
                count++;
    printf("capitals: %d\n", count);
    return 0;
}
