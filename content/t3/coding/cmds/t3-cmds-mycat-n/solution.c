#include <stdio.h>
#include <string.h>
#include <errno.h>

static long number;
static int numbered, at_start = 1;

static void copy(FILE *f)
{
    int c;

    while ((c = fgetc(f)) != EOF) {
        if (numbered && at_start)
            printf("%6ld\t", ++number);
        putchar(c);
        at_start = (c == '\n');
    }
}

int main(int argc, char *argv[])
{
    int status = 0, first = 1;

    if (argc > 1 && strcmp(argv[1], "-n") == 0) {
        numbered = 1;
        first = 2;
    }
    if (first >= argc) {
        copy(stdin);
        return 0;
    }
    for (int i = first; i < argc; i++) {
        FILE *f = strcmp(argv[i], "-") == 0 ? stdin : fopen(argv[i], "r");
        if (f == NULL) {
            fprintf(stderr, "mycat: %s: %s\n", argv[i], strerror(errno));
            status = 1;
            continue;
        }
        copy(f);
        if (f != stdin)
            fclose(f);
    }
    return status;
}
