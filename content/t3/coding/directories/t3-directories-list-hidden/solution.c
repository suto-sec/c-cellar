#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

static int cmp(const void *a, const void *b)
{
    return strcmp(*(char * const *) a, *(char * const *) b);
}

int main(int argc, char *argv[])
{
    DIR *d;
    struct dirent *e;
    char *names[1024];
    int n = 0;

    if (argc != 2)
        return 2;
    d = opendir(argv[1]);
    if (d == NULL)
        return 1;
    while ((e = readdir(d)) != NULL && n < 1024) {
        if (e->d_name[0] != '.' || strcmp(e->d_name, ".") == 0 || strcmp(e->d_name, "..") == 0)
            continue;
        names[n++] = strdup(e->d_name);
    }
    closedir(d);
    qsort(names, (size_t) n, sizeof(char *), cmp);
    for (int i = 0; i < n; i++) {
        puts(names[i]);
        free(names[i]);
    }
    return 0;
}
