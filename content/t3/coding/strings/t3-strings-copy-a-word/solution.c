#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    char copy[100];

    if (argc != 2)
        return 2;
    strcpy(copy, argv[1]);
    copy[0] = '*';
    printf("original: %s\ncopy: %s\n", argv[1], copy);
    return 0;
}
