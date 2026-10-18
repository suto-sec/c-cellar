#include <stdio.h>

int main(int argc, char *argv[])
{
    FILE *f;
    char buf[9];
    size_t n;

    if (argc != 2)
        return 2;
    f = fopen(argv[1], "r");
    if (f == NULL) {
        fprintf(stderr, "cannot open %s\n", argv[1]);
        return 1;
    }
    n = fread(buf, 1, 8, f);
    buf[n] = '\0';
    printf("read %zu bytes: %s\n", n, buf);
    fclose(f);
    return 0;
}
