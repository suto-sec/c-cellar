#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char *argv[])
{
    char *end;
    long v;

    if (argc != 2)
        return 2;
    errno = 0;
    v = strtol(argv[1], &end, 10);
    if (errno == ERANGE) {
        printf("error: out of range\n");
        return 1;
    }
    if (end == argv[1] || *end != '\0') {
        printf("error: not a number\n");
        return 1;
    }
    printf("%ld\n", v);
    return 0;
}
