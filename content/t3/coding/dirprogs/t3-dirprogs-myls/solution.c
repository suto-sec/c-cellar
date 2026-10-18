#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>

static int compare(const void *a, const void *b)
{
    return strcmp(*(char * const *) a, *(char * const *) b);
}

static int is_dir(const char *path)
{
    DIR *d = opendir(path);

    if (d == NULL)
        return 0;
    closedir(d);
    return 1;
}

int main(int argc, char *argv[])
{
    const char *dir = NULL;
    int all = 0;
    DIR *d;
    struct dirent *e;
    char **names = NULL;
    size_t n = 0, cap = 0;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-a") == 0)
            all = 1;
        else if (argv[i][0] != '-' && dir == NULL)
            dir = argv[i];
        else {
            fprintf(stderr, "usage: myls [-a] [DIR]\n");
            return 2;
        }
    }
    if (dir == NULL)
        dir = ".";
    if ((d = opendir(dir)) == NULL) {
        fprintf(stderr, "myls: %s: %s\n", dir, strerror(errno));
        return 1;
    }
    while ((e = readdir(d)) != NULL) {
        if (strcmp(e->d_name, ".") == 0 || strcmp(e->d_name, "..") == 0)
            continue;
        if (e->d_name[0] == '.' && !all)
            continue;
        if (n == cap) {
            cap = cap ? cap * 2 : 16;
            names = realloc(names, cap * sizeof(char *));
            if (names == NULL)
                return 1;
        }
        names[n++] = strdup(e->d_name);
    }
    closedir(d);
    if (n > 1)
        qsort(names, n, sizeof(char *), compare);
    for (size_t i = 0; i < n; i++) {
        char path[4096];
        snprintf(path, sizeof(path), "%s/%s", dir, names[i]);
        printf("%s%s\n", names[i], is_dir(path) ? "/" : "");
        free(names[i]);
    }
    free(names);
    return 0;
}
