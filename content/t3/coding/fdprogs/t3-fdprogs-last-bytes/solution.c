#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int fd, n;
    char buf[4096];
    off_t size, start;
    ssize_t r;

    if (argc != 3 || argv[2][0] < '0' || argv[2][0] > '9' || (n = atoi(argv[2])) < 0) {
        fprintf(stderr, "usage: lastbytes FILE N\n");
        return 2;
    }
    if ((fd = open(argv[1], O_RDONLY)) == -1) {
        fprintf(stderr, "lastbytes: %s: %s\n", argv[1], strerror(errno));
        return 1;
    }
    size = lseek(fd, 0, SEEK_END);
    start = size > n ? size - n : 0;
    lseek(fd, start, SEEK_SET);
    while ((r = read(fd, buf, sizeof(buf))) > 0)
        if (write(1, buf, (size_t) r) != r)
            return 1;
    close(fd);
    return 0;
}
