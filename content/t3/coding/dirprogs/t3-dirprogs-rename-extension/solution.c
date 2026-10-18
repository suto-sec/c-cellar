#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    DIR *d;
    struct dirent *e;
    char **names = NULL;
    size_t n = 0, cap = 0, sl;
    char suffix[256];
    int status = 0;

    if (argc != 4) {
        fprintf(stderr, "usage: renameext DIR OLD NEW\n");
        return 2;
    }
    if ((d = opendir(argv[1])) == NULL) {
        fprintf(stderr, "renameext: %s: %s\n", argv[1], strerror(errno));
        return 1;
    }
    snprintf(suffix, sizeof(suffix), ".%s", argv[2]);
    sl = strlen(suffix);
    while ((e = readdir(d)) != NULL) {
        size_t nl = strlen(e->d_name);
        if (nl > sl && strcmp(e->d_name + nl - sl, suffix) == 0) {
            if (n == cap) {
                cap = cap ? cap * 2 : 16;
                names = realloc(names, cap * sizeof(char *));
                if (names == NULL)
                    return 1;
            }
            names[n++] = strdup(e->d_name);
        }
    }
    closedir(d);
    for (size_t i = 0; i < n; i++) {
        char from[4096], to[4096], newname[1024];
        size_t nl = strlen(names[i]);

        snprintf(newname, sizeof(newname), "%.*s.%s", (int) (nl - sl), names[i], argv[3]);
        snprintf(from, sizeof(from), "%s/%s", argv[1], names[i]);
        snprintf(to, sizeof(to), "%s/%s", argv[1], newname);
        if (access(to, F_OK) == 0) {
            fprintf(stderr, "renameext: %s: target exists\n", names[i]);
            status = 1;
        } else if (rename(from, to) == -1) {
            fprintf(stderr, "renameext: %s: %s\n", names[i], strerror(errno));
            status = 1;
        } else {
            printf("%s -> %s\n", names[i], newname);
        }
        free(names[i]);
    }
    free(names);
    return status;
}
