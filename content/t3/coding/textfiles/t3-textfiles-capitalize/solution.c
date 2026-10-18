#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

int main(int argc, char *argv[])
{
    FILE *f;
    int c, inword = 0;

    if (argc != 2) {
        fprintf(stderr, "usage: capitalize FILE\n");
        return 2;
    }
    if ((f = fopen(argv[1], "r")) == NULL) {
        fprintf(stderr, "capitalize: %s: %s\n", argv[1], strerror(errno));
        return 1;
    }
    while ((c = fgetc(f)) != EOF) {
        if (isalpha(c)) {
            putchar(inword ? tolower(c) : toupper(c));
            inword = 1;
        } else {
            putchar(c);
            inword = 0;
        }
    }
    fclose(f);
    return 0;
}
