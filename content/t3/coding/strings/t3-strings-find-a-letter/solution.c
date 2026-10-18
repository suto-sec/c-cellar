#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    char *p;

    if (argc != 3)
        return 2;
    p = strchr(argv[1], argv[2][0]);
    if (p == NULL)
        printf("not found\n");
    else
        printf("%ld\n", (long) (p - argv[1]));
    return 0;
}
