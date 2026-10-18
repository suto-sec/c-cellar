#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>

static long files, dirs;

static void walk(const char *dir)
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
        if ((probe = opendir(path)) != NULL) {
            closedir(probe);
            dirs++;
            walk(path);
        } else {
            files++;
        }
    }
    closedir(d);
}

int main(int argc, char *argv[])
{
    DIR *d;

    if (argc != 2) {
        fprintf(stderr, "usage: treecount DIR\n");
        return 2;
    }
    if ((d = opendir(argv[1])) == NULL) {
        fprintf(stderr, "treecount: %s: %s\n", argv[1], strerror(errno));
        return 1;
    }
    closedir(d);
    walk(argv[1]);
    printf("files: %ld\ndirectories: %ld\n", files, dirs);
    return 0;
}
