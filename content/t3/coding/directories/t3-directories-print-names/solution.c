#include <stdio.h>
#include <dirent.h>

int main(int argc, char *argv[])
{
    DIR *d;
    struct dirent *e;

    if (argc != 2)
        return 2;
    d = opendir(argv[1]);
    if (d == NULL) {
        fprintf(stderr, "cannot open %s\n", argv[1]);
        return 1;
    }
    while ((e = readdir(d)) != NULL)
        printf("%s\n", e->d_name);
    closedir(d);
    return 0;
}
