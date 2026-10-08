#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    if (argc != 3)
        return 1;
    int fd = creat(argv[1], S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (fd < 0)
        return 1;
    if (write(fd, argv[2], strlen(argv[2])) < 0 || write(fd, "\n", 1) < 0)
        return 1;
    close(fd);
    return 0;
}
