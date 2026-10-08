#include <stdio.h>

int main(int argc, char *argv[])
{
    FILE *f;

    if (argc != 3)
        return 2;
    f = fopen(argv[1], "a");
    if (f == NULL) {
        fprintf(stderr, "cannot open %s\n", argv[1]);
        return 1;
    }
    fprintf(f, "%s\n", argv[2]);
    fclose(f);
    return 0;
}
