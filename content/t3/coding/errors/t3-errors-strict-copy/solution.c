#include <stdio.h>
#include <string.h>
#include <errno.h>

int main(int argc, char *argv[])
{
    FILE *in, *out;
    char buf[512];
    size_t n;

    if (argc != 3) {
        fprintf(stderr, "usage: strictcopy SRC DST\n");
        return 3;
    }
    in = fopen(argv[1], "rb");
    if (in == NULL) {
        fprintf(stderr, "cannot open %s: %s\n", argv[1], strerror(errno));
        return 1;
    }
    out = fopen(argv[2], "wb");
    if (out == NULL) {
        fprintf(stderr, "cannot create %s: %s\n", argv[2], strerror(errno));
        fclose(in);
        return 2;
    }
    while ((n = fread(buf, 1, sizeof(buf), in)) > 0)
        if (fwrite(buf, 1, n, out) != n) {
            fclose(in);
            fclose(out);
            return 2;
        }
    fclose(in);
    fclose(out);
    return 0;
}
