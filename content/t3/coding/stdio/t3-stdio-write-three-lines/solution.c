#include <stdio.h>

int main(int argc, char *argv[])
{
    FILE *f;

    if (argc != 2)
        return 2;
    f = fopen(argv[1], "w");
    if (f == NULL) {
        fprintf(stderr, "cannot create %s\n", argv[1]);
        return 1;
    }
    for (int i = 1; i <= 3; i++)
        fprintf(f, "line %d\n", i);
    fclose(f);
    return 0;
}
