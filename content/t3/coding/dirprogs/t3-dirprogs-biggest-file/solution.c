#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>

static char best[4096];
static long best_size = -1;

static long file_size(const char *path)
{
    int fd = open(path, O_RDONLY);
    off_t size;

    if (fd == -1)
        return 0;
    size = lseek(fd, 0, SEEK_END);
    close(fd);
    return size < 0 ? 0 : (long) size;
}

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
            walk(path);
        } else {
            long size = file_size(path);
            if (size > best_size || (size == best_size && strcmp(path, best) < 0)) {
                best_size = size;
                strcpy(best, path);
            }
        }
    }
    closedir(d);
}

int main(int argc, char *argv[])
{
    DIR *d;

    if (argc != 2) {
        fprintf(stderr, "usage: biggest DIR\n");
        return 2;
    }
    if ((d = opendir(argv[1])) == NULL) {
        fprintf(stderr, "biggest: %s: %s\n", argv[1], strerror(errno));
        return 1;
    }
    closedir(d);
    walk(argv[1]);
    if (best_size < 0) {
        fprintf(stderr, "biggest: no files in %s\n", argv[1]);
        return 1;
    }
    printf("%s %ld\n", best, best_size);
    return 0;
}
