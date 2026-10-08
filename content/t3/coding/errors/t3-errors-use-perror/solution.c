#include <stdio.h>

int main(int argc, char *argv[])
{
    FILE *f;

    if (argc != 2)
        return 2;
    f = fopen(argv[1], "r");
    if (f == NULL) {
        perror(argv[1]);
        return 1;
    }
    printf("opened %s\n", argv[1]);
    fclose(f);
    return 0;
}
