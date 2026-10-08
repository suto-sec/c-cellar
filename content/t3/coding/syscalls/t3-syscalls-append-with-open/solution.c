#include <string.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int fd;

    if (argc != 3)
        return 2;
    fd = open(argv[1], O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd < 0)
        return 1;
    write(fd, argv[2], strlen(argv[2]));
    write(fd, "\n", 1);
    close(fd);
    return 0;
}
