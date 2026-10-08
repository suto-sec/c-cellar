#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[], char *envp[])
{
    if (argc != 2)
        return 1;
    size_t n = strlen(argv[1]);

    for (int i = 0; envp[i] != NULL; i++)
        if (strncmp(envp[i], argv[1], n) == 0)
            puts(envp[i]);
    return 0;
}
