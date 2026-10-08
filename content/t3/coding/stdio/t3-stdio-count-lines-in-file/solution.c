#include <stdio.h>

int main(int argc, char *argv[])
{
    FILE *f;
    int c, lines = 0;

    if (argc != 2)
        return 2;
    f = fopen(argv[1], "r");
    if (f == NULL) {
        fprintf(stderr, "cannot open %s\n", argv[1]);
        return 1;
    }
    while ((c = fgetc(f)) != EOF)
        if (c == '\n')
            lines++;
    fclose(f);
    printf("%d\n", lines);
    return 0;
}
