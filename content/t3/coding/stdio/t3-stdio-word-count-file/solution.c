#include <stdio.h>
#include <ctype.h>

int main(int argc, char *argv[])
{
    FILE *f;
    int c, inword = 0;
    long lines = 0, words = 0, bytes = 0;

    if (argc != 2)
        return 2;
    f = fopen(argv[1], "rb");
    if (f == NULL) {
        fprintf(stderr, "cannot open %s\n", argv[1]);
        return 1;
    }
    while ((c = fgetc(f)) != EOF) {
        bytes++;
        if (c == '\n')
            lines++;
        if (isspace(c))
            inword = 0;
        else if (!inword) {
            inword = 1;
            words++;
        }
    }
    fclose(f);
    printf("lines=%ld words=%ld bytes=%ld\n", lines, words, bytes);
    return 0;
}
