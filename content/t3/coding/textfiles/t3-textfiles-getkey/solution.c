#include <stdio.h>
#include <string.h>
#include <errno.h>

int main(int argc, char *argv[])
{
    FILE *f;
    char line[1024];
    size_t klen;

    if (argc != 3) {
        fprintf(stderr, "usage: getkey FILE KEY\n");
        return 2;
    }
    if ((f = fopen(argv[1], "r")) == NULL) {
        fprintf(stderr, "getkey: %s: %s\n", argv[1], strerror(errno));
        return 2;
    }
    klen = strlen(argv[2]);
    while (fgets(line, sizeof(line), f) != NULL) {
        line[strcspn(line, "\n")] = '\0';
        if (line[0] == '#' || line[0] == '\0')
            continue;
        if (strncmp(line, argv[2], klen) == 0 && line[klen] == '=') {
            puts(line + klen + 1);
            fclose(f);
            return 0;
        }
    }
    fclose(f);
    fprintf(stderr, "getkey: %s: not found\n", argv[2]);
    return 1;
}
