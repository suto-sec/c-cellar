#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main(int argc, char *argv[])
{
    FILE *in, *out = NULL;
    char line[1024], name[4096] = "";
    int n, part = 0;
    long inpart = 0;

    if (argc != 3 || (n = atoi(argv[2])) <= 0) {
        fprintf(stderr, "usage: splitfile FILE N\n");
        return 2;
    }
    if ((in = fopen(argv[1], "r")) == NULL) {
        fprintf(stderr, "splitfile: %s: %s\n", argv[1], strerror(errno));
        return 1;
    }
    while (fgets(line, sizeof(line), in) != NULL) {
        if (out == NULL) {
            snprintf(name, sizeof(name), "%s.part%d", argv[1], ++part);
            if ((out = fopen(name, "w")) == NULL) {
                fprintf(stderr, "splitfile: %s: %s\n", name, strerror(errno));
                return 1;
            }
            inpart = 0;
        }
        fputs(line, out);
        if (++inpart == n) {
            fclose(out);
            out = NULL;
            printf("%s: %ld lines\n", name, inpart);
        }
    }
    if (out != NULL) {
        fclose(out);
        printf("%s: %ld lines\n", name, inpart);
    }
    fclose(in);
    return 0;
}
