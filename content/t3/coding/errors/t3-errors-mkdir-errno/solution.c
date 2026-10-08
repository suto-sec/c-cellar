#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
        return 2;
    if (mkdir(argv[1], 0755) == 0)
        printf("created\n");
    else if (errno == EEXIST)
        printf("exists\n");
    else if (errno == ENOENT)
        printf("no parent\n");
    else if (errno == EACCES)
        printf("denied\n");
    else
        printf("error: %s\n", strerror(errno));
    return 0;
}
