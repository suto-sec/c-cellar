#include <stdio.h>

int main(int argc, char *argv[])
{
    FILE *in, *out;
    unsigned char buf[512];
    size_t n;
    long total = 0;

    if (argc != 3)
        return 2;
    in = fopen(argv[1], "rb");
    if (in == NULL) {
        fprintf(stderr, "cannot open %s\n", argv[1]);
        return 1;
    }
    out = fopen(argv[2], "wb");
    if (out == NULL) {
        fclose(in);
        return 1;
    }
    while ((n = fread(buf, 1, sizeof(buf), in)) > 0) {
        fwrite(buf, 1, n, out);
        total += (long) n;
    }
    fclose(in);
    fclose(out);
    printf("copied %ld bytes\n", total);
    return 0;
}
