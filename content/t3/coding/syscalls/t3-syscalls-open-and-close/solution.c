#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int fd;

    if (argc != 2)
        return 2;
    fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        printf("cannot open %s\n", argv[1]);
        return 1;
    }
    printf("opened with descriptor %d\n", fd);
    close(fd);
    return 0;
}
