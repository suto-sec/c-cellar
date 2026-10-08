#include <stdio.h>
#include <errno.h>
#include <dirent.h>

int main(int argc, char *argv[])
{
    DIR *d;

    if (argc != 2)
        return 2;
    d = opendir(argv[1]);
    if (d != NULL) {
        closedir(d);
        printf("directory\n");
    } else if (errno == ENOTDIR)
        printf("not a directory\n");
    else if (errno == ENOENT)
        printf("missing\n");
    else
        printf("error\n");
    return 0;
}
