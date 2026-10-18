#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int fd1, fd2, fd3, fd4;
    char a[4] = "", b[4] = "", c[4] = "", d[4] = "";

    if (argc != 2) {
        fprintf(stderr, "usage: sharedoffset FILE\n");
        return 2;
    }
    fd1 = open(argv[1], O_RDONLY);
    fd3 = open(argv[1], O_RDONLY);
    fd4 = open(argv[1], O_RDONLY);
    if (fd1 == -1 || fd3 == -1 || fd4 == -1) {
        fprintf(stderr, "sharedoffset: %s: %s\n", argv[1], strerror(errno));
        return 1;
    }
    fd2 = dup(fd1);
    if (read(fd1, a, 3) != 3 || read(fd2, b, 3) != 3 || read(fd3, c, 3) != 3 || read(fd4, d, 3) != 3) {
        fprintf(stderr, "sharedoffset: %s: too short\n", argv[1]);
        return 1;
    }
    printf("dup: %s %s\nopen: %s %s\n", a, b, c, d);
    return 0;
}
