#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>

static long total;

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
            total += file_size(path);
        }
    }
    closedir(d);
}

int main(int argc, char *argv[])
{
    DIR *d;

    if (argc != 2) {
        fprintf(stderr, "usage: dusum DIR\n");
        return 2;
    }
    if ((d = opendir(argv[1])) == NULL) {
        fprintf(stderr, "dusum: %s: %s\n", argv[1], strerror(errno));
        return 1;
    }
    closedir(d);
    walk(argv[1]);
    printf("%ld\n", total);
    return 0;
}
