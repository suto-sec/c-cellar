#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int c, del = 0;
    const char *from, *to = NULL;

    if (argc == 3 && strcmp(argv[1], "-d") == 0) {
        del = 1;
        from = argv[2];
    } else if (argc == 3) {
        from = argv[1];
        to = argv[2];
        if (strlen(from) != strlen(to)) {
            fprintf(stderr, "mytr: FROM and TO must have the same length\n");
            return 2;
        }
    } else {
        fprintf(stderr, "usage: mytr FROM TO | mytr -d SET\n");
        return 2;
    }
    while ((c = getchar()) != EOF) {
        const char *p = c != 0 ? strchr(from, c) : NULL;
        if (p == NULL)
            putchar(c);
        else if (!del)
            putchar(to[p - from]);
    }
    return 0;
}
