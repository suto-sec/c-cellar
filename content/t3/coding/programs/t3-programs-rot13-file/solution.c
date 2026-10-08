#include <stdio.h>

int main(int argc, char *argv[])
{
    FILE *in, *out;
    int c;

    if (argc != 3)
        return 3;
    in = fopen(argv[1], "r");
    if (in == NULL) {
        fprintf(stderr, "cannot open %s\n", argv[1]);
        return 1;
    }
    out = fopen(argv[2], "w");
    if (out == NULL) {
        fclose(in);
        return 2;
    }
    while ((c = fgetc(in)) != EOF) {
        if (c >= 'a' && c <= 'z')
            c = 'a' + (c - 'a' + 13) % 26;
        else if (c >= 'A' && c <= 'Z')
            c = 'A' + (c - 'A' + 13) % 26;
        fputc(c, out);
    }
    fclose(in);
    fclose(out);
    return 0;
}
