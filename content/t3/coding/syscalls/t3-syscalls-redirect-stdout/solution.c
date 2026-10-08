#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int fd;

    if (argc != 2)
        return 2;
    printf("before\n");
    fflush(stdout);
    fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0)
        return 1;
    dup2(fd, STDOUT_FILENO);
    close(fd);
    printf("inside the file\n");
    return 0;
}
