#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main(int argc, char *argv[])
{
    FILE *f;
    char *ext;
    char (*w)[32];
    int n;

    if (argc != 3) {
        fprintf(stderr, "usage: sortwords FILE.txt N\n");
        return 2;
    }
    ext = strrchr(argv[1], '.');
    if (ext == NULL || strcmp(ext, ".txt") != 0) {
        fprintf(stderr, "sortwords: %s: not a .txt file\n", argv[1]);
        return 2;
    }
    n = atoi(argv[2]);
    if (n <= 0) {
        fprintf(stderr, "usage: sortwords FILE.txt N\n");
        return 2;
    }
    if ((f = fopen(argv[1], "r")) == NULL) {
        fprintf(stderr, "sortwords: %s: %s\n", argv[1], strerror(errno));
        return 1;
    }
    w = calloc((size_t) n, 32);
    if (w == NULL)
        return 1;
    for (int i = 0; i < n; i++)
        if (fscanf(f, "%31s", w[i]) != 1)
            break;
    fclose(f);
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - 1 - i; j++)
            if (strcmp(w[j], w[j + 1]) > 0) {
                char tmp[32];
                strcpy(tmp, w[j]);
                strcpy(w[j], w[j + 1]);
                strcpy(w[j + 1], tmp);
            }
    for (int i = 0; i < n; i++)
        puts(w[i]);
    free(w);
    return 0;
}
