#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    char buf[32];
    ssize_t got;

    if (argc != 2)
        return 1;
    int fd = open(argv[1], O_RDWR);
    if (fd < 0)
        return 1;
    got = read(fd, buf, sizeof buf - 1);
    if (got <= 0)
        return 1;
    buf[got] = '\0';
    int value = atoi(buf) + 1;
    int len = snprintf(buf, sizeof buf, "%d\n", value);

    lseek(fd, 0, SEEK_SET);
    if (write(fd, buf, len) != len)
        return 1;
    close(fd);
    printf("%d\n", value);
    return 0;
}
