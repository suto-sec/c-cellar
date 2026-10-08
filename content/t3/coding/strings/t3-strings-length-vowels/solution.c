#include <stdio.h>
#include <string.h>

int main(void)
{
    char line[1024];
    int vowels = 0;

    if (fgets(line, sizeof(line), stdin) == NULL)
        return 1;
    line[strcspn(line, "\n")] = '\0';
    for (char *p = line; *p != '\0'; p++)
        if (strchr("aeiouAEIOU", *p) != NULL)
            vowels++;
    printf("length: %zu\nvowels: %d\n", strlen(line), vowels);
    return 0;
}
