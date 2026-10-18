#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int fd;
    unsigned char buf[16];
    long offset = 0;

    if (argc != 2) {
        fprintf(stderr, "usage: hexd FILE\n");
        return 2;
    }
    if ((fd = open(argv[1], O_RDONLY)) == -1) {
        fprintf(stderr, "hexd: %s: %s\n", argv[1], strerror(errno));
        return 1;
    }
    for (;;) {
        int n = 0;
        ssize_t r;
        while (n < 16 && (r = read(fd, buf + n, (size_t) (16 - n))) > 0)
            n += (int) r;
        if (n == 0)
            break;
        printf("%08lx:", offset);
        for (int i = 0; i < n; i++)
            printf(" %02x", buf[i]);
        putchar('\n');
        offset += n;
        if (n < 16)
            break;
    }
    close(fd);
    return 0;
}
