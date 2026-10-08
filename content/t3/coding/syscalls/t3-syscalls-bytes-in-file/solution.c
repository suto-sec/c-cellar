#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int fd;
    off_t size;

    if (argc != 2)
        return 2;
    fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        fprintf(stderr, "%s: %s\n", argv[1], strerror(errno));
        return 1;
    }
    size = lseek(fd, 0, SEEK_END);
    close(fd);
    printf("%ld\n", (long) size);
    return 0;
}
