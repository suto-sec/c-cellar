#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    char buf[256];
    int in, out;
    ssize_t n;

    if (argc != 3) {
        fprintf(stderr, "usage: %s SRC DST\n", argv[0]);
        return 2;
    }
    if ((in = open(argv[1], O_RDONLY)) < 0) {
        fprintf(stderr, "%s: %s\n", argv[1], strerror(errno));
        return 1;
    }
    if ((out = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644)) < 0) {
        fprintf(stderr, "%s: %s\n", argv[2], strerror(errno));
        close(in);
        return 1;
    }
    while ((n = read(in, buf, sizeof(buf))) > 0) {
        if (write(out, buf, n) != n) {
            fprintf(stderr, "write: %s\n", strerror(errno));
            close(in);
            close(out);
            return 1;
        }
    }
    close(in);
    close(out);
    return n < 0 ? 1 : 0;
}
