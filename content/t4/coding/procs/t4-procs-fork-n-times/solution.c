#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int n, i;

    if (argc != 2)
        return 2;
    n = atoi(argv[1]);
    for (i = 0; i < n; i++)
        fork();
    printf("x\n");
    return 0;
}
