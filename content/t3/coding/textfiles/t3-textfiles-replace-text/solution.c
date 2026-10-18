#include <stdio.h>
#include <string.h>
#include <errno.h>

int main(int argc, char *argv[])
{
    FILE *f;
    char line[1024];
    size_t olen;

    if (argc != 4 || argv[2][0] == '\0') {
        fprintf(stderr, "usage: replace FILE OLD NEW\n");
        return 2;
    }
    if ((f = fopen(argv[1], "r")) == NULL) {
        fprintf(stderr, "replace: %s: %s\n", argv[1], strerror(errno));
        return 1;
    }
    olen = strlen(argv[2]);
    while (fgets(line, sizeof(line), f) != NULL) {
        char *p = line, *hit;
        while ((hit = strstr(p, argv[2])) != NULL) {
            fwrite(p, 1, (size_t) (hit - p), stdout);
            fputs(argv[3], stdout);
            p = hit + olen;
        }
        fputs(p, stdout);
    }
    fclose(f);
    return 0;
}
