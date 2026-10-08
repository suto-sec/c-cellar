#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    char buf[256];
    ssize_t n;
    int fd;

    if (argc != 2)
        return 2;
    fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0)
        return 1;
    while ((n = read(STDIN_FILENO, buf, sizeof(buf))) > 0) {
        write(STDOUT_FILENO, buf, (size_t) n);
        write(fd, buf, (size_t) n);
    }
    close(fd);
    return 0;
}
