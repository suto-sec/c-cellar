#include <stdio.h>
#include <string.h>
#include <errno.h>

int main(int argc, char *argv[])
{
    FILE *a, *b;
    long byte = 0, line = 1;
    int ca, cb;

    if (argc != 3) {
        fprintf(stderr, "usage: mycmp FILE1 FILE2\n");
        return 2;
    }
    if ((a = fopen(argv[1], "rb")) == NULL) {
        fprintf(stderr, "mycmp: %s: %s\n", argv[1], strerror(errno));
        return 2;
    }
    if ((b = fopen(argv[2], "rb")) == NULL) {
        fprintf(stderr, "mycmp: %s: %s\n", argv[2], strerror(errno));
        return 2;
    }
    for (;;) {
        ca = fgetc(a);
        cb = fgetc(b);
        if (ca == EOF && cb == EOF)
            return 0;
        if (ca == EOF || cb == EOF) {
            fprintf(stderr, "mycmp: EOF on %s after byte %ld, line %ld\n", ca == EOF ? argv[1] : argv[2], byte, line);
            return 1;
        }
        byte++;
        if (ca != cb) {
            printf("%s %s differ: byte %ld, line %ld\n", argv[1], argv[2], byte, line);
            return 1;
        }
        if (ca == '\n')
            line++;
    }
}
