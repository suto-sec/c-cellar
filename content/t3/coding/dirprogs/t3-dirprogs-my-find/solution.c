#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>

static void walk(const char *dir, const char *name)
{
    DIR *d = opendir(dir);
    struct dirent *e;

    if (d == NULL)
        return;
    while ((e = readdir(d)) != NULL) {
        char path[4096];
        DIR *probe;

        if (strcmp(e->d_name, ".") == 0 || strcmp(e->d_name, "..") == 0)
            continue;
        snprintf(path, sizeof(path), "%s/%s", dir, e->d_name);
        if (strcmp(e->d_name, name) == 0)
            puts(path);
        if ((probe = opendir(path)) != NULL) {
            closedir(probe);
            walk(path, name);
        }
    }
    closedir(d);
}

int main(int argc, char *argv[])
{
    DIR *d;

    if (argc != 3) {
        fprintf(stderr, "usage: myfind DIR NAME\n");
        return 2;
    }
    if ((d = opendir(argv[1])) == NULL) {
        fprintf(stderr, "myfind: %s: %s\n", argv[1], strerror(errno));
        return 1;
    }
    closedir(d);
    walk(argv[1], argv[2]);
    return 0;
}
