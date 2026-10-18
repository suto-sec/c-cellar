#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int a, b, c, d, e, f;

    if (argc != 2) {
        fprintf(stderr, "usage: fdnums FILE\n");
        return 2;
    }
    if ((a = open(argv[1], O_RDONLY)) == -1 || (b = open(argv[1], O_RDONLY)) == -1) {
        fprintf(stderr, "fdnums: %s: %s\n", argv[1], strerror(errno));
        return 1;
    }
    printf("a=%d\nb=%d\n", a, b);
    close(a);
    c = open(argv[1], O_RDONLY);
    d = dup(b);
    e = dup2(b, 10);
    f = open(argv[1], O_RDONLY);
    printf("c=%d\nd=%d\ne=%d\nf=%d\n", c, d, e, f);
    return 0;
}
