#include <fcntl.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    const char *first = "redirected\n";
    const char *second = "back on screen\n";

    if (argc != 2)
        return 1;
    int saved = dup(1);
    int fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (saved < 0 || fd < 0)
        return 1;
    dup2(fd, 1);
    close(fd);
    if (write(1, first, strlen(first)) < 0)
        return 1;
    dup2(saved, 1);
    close(saved);
    if (write(1, second, strlen(second)) < 0)
        return 1;
    return 0;
}
