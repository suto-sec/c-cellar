#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int fd;

    if (argc != 4)
        return 2;
    fd = open(argv[1], O_WRONLY);
    if (fd < 0)
        return 1;
    lseek(fd, atol(argv[2]), SEEK_SET);
    write(fd, argv[3], strlen(argv[3]));
    close(fd);
    return 0;
}
