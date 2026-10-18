#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>

static void walk(const char *dir)
{
    DIR *d = opendir(dir);
    struct dirent *e;
    int entries = 0;

    if (d == NULL)
        return;
    while ((e = readdir(d)) != NULL) {
        char path[4096];
        DIR *probe;

        if (strcmp(e->d_name, ".") == 0 || strcmp(e->d_name, "..") == 0)
            continue;
        entries++;
        snprintf(path, sizeof(path), "%s/%s", dir, e->d_name);
        if ((probe = opendir(path)) != NULL) {
            closedir(probe);
            walk(path);
        }
    }
    closedir(d);
    if (entries == 0)
        puts(dir);
}

int main(int argc, char *argv[])
{
    DIR *d;

    if (argc != 2) {
        fprintf(stderr, "usage: emptydirs DIR\n");
        return 2;
    }
    if ((d = opendir(argv[1])) == NULL) {
        fprintf(stderr, "emptydirs: %s: %s\n", argv[1], strerror(errno));
        return 1;
    }
    closedir(d);
    walk(argv[1]);
    return 0;
}
