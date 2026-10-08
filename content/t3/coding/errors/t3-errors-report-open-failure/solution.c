#include <stdio.h>
#include <string.h>
#include <errno.h>

int main(int argc, char *argv[])
{
    FILE *f;

    if (argc != 2)
        return 2;
    f = fopen(argv[1], "r");
    if (f == NULL) {
        fprintf(stderr, "cannot open %s: %s\n", argv[1], strerror(errno));
        return 1;
    }
    printf("opened %s\n", argv[1]);
    fclose(f);
    return 0;
}
