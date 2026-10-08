#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>

int main(int argc, char *argv[])
{
    DIR *d;
    struct dirent *e;
    int n = 0;

    if (argc != 2)
        return 2;
    d = opendir(argv[1]);
    if (d == NULL) {
        fprintf(stderr, "%s: %s\n", argv[1], strerror(errno));
        return 1;
    }
    while ((e = readdir(d)) != NULL)
        if (strcmp(e->d_name, ".") != 0 && strcmp(e->d_name, "..") != 0)
            n++;
    closedir(d);
    printf("%d\n", n);
    return 0;
}
