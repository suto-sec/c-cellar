#include <stdio.h>

int main(int argc, char *argv[])
{
    FILE *f;

    if (argc != 2)
        return 2;
    f = fopen(argv[1], "rb");
    if (f == NULL) {
        fprintf(stderr, "cannot open %s\n", argv[1]);
        return 1;
    }
    fseek(f, 0, SEEK_END);
    printf("%ld\n", ftell(f));
    fclose(f);
    return 0;
}
