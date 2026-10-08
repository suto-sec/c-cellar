#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>

static int cmp_names(const void *a, const void *b)
{
    return strcmp(*(char * const *) a, *(char * const *) b);
}

int main(int argc, char *argv[])
{
    DIR *d;
    struct dirent *e;
    char **names = NULL;
    size_t n = 0, cap = 0;

    if (argc != 2) {
        fprintf(stderr, "usage: %s DIR\n", argv[0]);
        return 2;
    }
    if ((d = opendir(argv[1])) == NULL) {
        fprintf(stderr, "%s: %s\n", argv[1], strerror(errno));
        return 1;
    }
    while ((e = readdir(d)) != NULL) {
        if (strcmp(e->d_name, ".") == 0 || strcmp(e->d_name, "..") == 0)
            continue;
        if (n == cap) {
            cap = cap ? cap * 2 : 8;
            names = realloc(names, cap * sizeof(char *));
            if (names == NULL)
                return 1;
        }
        names[n++] = strdup(e->d_name);
    }
    closedir(d);
    qsort(names, n, sizeof(char *), cmp_names);
    for (size_t i = 0; i < n; i++) {
        puts(names[i]);
        free(names[i]);
    }
    free(names);
    return 0;
}
