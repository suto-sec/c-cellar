#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int n = 1, verbose = 0, nfiles = 0;
    char *files[64];

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-n") == 0 && i + 1 < argc)
            n = atoi(argv[++i]);
        else if (strcmp(argv[i], "-v") == 0)
            verbose = 1;
        else if (nfiles < 64)
            files[nfiles++] = argv[i];
    }
    printf("n=%d verbose=%s files=", n, verbose ? "yes" : "no");
    if (nfiles == 0)
        printf("none");
    for (int i = 0; i < nfiles; i++)
        printf("%s%s", i ? " " : "", files[i]);
    printf("\n");
    return 0;
}
