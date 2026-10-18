#include <stdio.h>
#include <dirent.h>

int main(int argc, char *argv[])
{
    DIR *d;

    if (argc != 2)
        return 2;
    d = opendir(argv[1]);
    if (d == NULL) {
        printf("cannot open %s\n", argv[1]);
        return 1;
    }
    printf("%s is a directory\n", argv[1]);
    closedir(d);
    return 0;
}
