#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int fd;

    if (argc != 2) {
        fprintf(stderr, "usage: logboth FILE\n");
        return 2;
    }
    printf("starting\n");
    fflush(stdout);
    if ((fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0644)) == -1) {
        fprintf(stderr, "logboth: %s: %s\n", argv[1], strerror(errno));
        return 1;
    }
    dup2(fd, 1);
    dup2(fd, 2);
    close(fd);
    printf("out line\n");
    fflush(stdout);
    fprintf(stderr, "err line\n");
    return 0;
}
