#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <sys/stat.h>

static int make(const char *p)
{
    DIR *d;

    if (mkdir(p, 0755) == 0)
        return 0;
    if (errno == EEXIST) {
        if ((d = opendir(p)) != NULL) {
            closedir(d);
            return 0;
        }
        errno = ENOTDIR;
    }
    return -1;
}

int main(int argc, char *argv[])
{
    char path[4096];

    if (argc != 2) {
        fprintf(stderr, "usage: mymkdirp PATH\n");
        return 2;
    }
    snprintf(path, sizeof(path), "%s", argv[1]);
    for (size_t i = 1; path[i]; i++) {
        if (path[i] == '/' && path[i - 1] != '/') {
            path[i] = '\0';
            if (make(path) == -1) {
                fprintf(stderr, "mymkdirp: %s: %s\n", argv[1], strerror(errno));
                return 1;
            }
            path[i] = '/';
        }
    }
    if (make(path) == -1) {
        fprintf(stderr, "mymkdirp: %s: %s\n", argv[1], strerror(errno));
        return 1;
    }
    return 0;
}
