#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int fd;
    off_t pos;
    char c;

    if (argc != 2)
        return 2;
    fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        fprintf(stderr, "%s: %s\n", argv[1], strerror(errno));
        return 1;
    }
    for (pos = lseek(fd, 0, SEEK_END) - 1; pos >= 0; pos--) {
        lseek(fd, pos, SEEK_SET);
        if (read(fd, &c, 1) == 1)
            write(STDOUT_FILENO, &c, 1);
    }
    close(fd);
    return 0;
}
