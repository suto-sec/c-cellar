#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int best = 0;

    for (int i = 1; i < argc; i++)
        if (best == 0 || strlen(argv[i]) > strlen(argv[best]))
            best = i;
    if (best == 0)
        printf("no arguments\n");
    else
        printf("%zu %s\n", strlen(argv[best]), argv[best]);
    return 0;
}
