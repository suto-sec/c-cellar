#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int flags, fd;

    if (argc != 4) {
        fprintf(stderr, "usage: fmode MODE FILE TEXT\n");
        return 2;
    }
    if (strcmp(argv[1], "r") == 0)
        flags = O_RDONLY;
    else if (strcmp(argv[1], "w") == 0)
        flags = O_WRONLY | O_CREAT | O_TRUNC;
    else if (strcmp(argv[1], "a") == 0)
        flags = O_WRONLY | O_CREAT | O_APPEND;
    else if (strcmp(argv[1], "r+") == 0)
        flags = O_RDWR;
    else if (strcmp(argv[1], "w+") == 0)
        flags = O_RDWR | O_CREAT | O_TRUNC;
    else if (strcmp(argv[1], "a+") == 0)
        flags = O_RDWR | O_CREAT | O_APPEND;
    else {
        fprintf(stderr, "usage: fmode MODE FILE TEXT\n");
        return 2;
    }
    if ((fd = open(argv[2], flags, 0644)) == -1) {
        fprintf(stderr, "fmode: %s: %s\n", argv[2], strerror(errno));
        return 1;
    }
    if (write(fd, argv[3], strlen(argv[3])) == -1) {
        fprintf(stderr, "fmode: write: %s\n", strerror(errno));
        close(fd);
        return 1;
    }
    close(fd);
    return 0;
}
