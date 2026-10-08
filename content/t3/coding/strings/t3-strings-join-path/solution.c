#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    char path[256];

    if (argc != 3)
        return 1;
    strcpy(path, argv[1]);
    strcat(path, "/");
    strcat(path, argv[2]);
    puts(path);
    return 0;
}
