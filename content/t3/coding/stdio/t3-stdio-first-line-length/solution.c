#include <stdio.h>

int main(int argc, char *argv[])
{
    FILE *f;
    int c;

    if (argc != 2)
        return 2;
    f = fopen(argv[1], "r");
    if (f == NULL) {
        fprintf(stderr, "cannot open %s\n", argv[1]);
        return 1;
    }
    while ((c = fgetc(f)) != EOF && c != '\n')
        ;
    printf("position: %ld\n", ftell(f));
    fclose(f);
    return 0;
}
