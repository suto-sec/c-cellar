#include <stdio.h>

int main(int argc, char *argv[])
{
    FILE *f;
    char line[256];
    int n = 0;

    if (argc != 2)
        return 2;
    f = fopen(argv[1], "r");
    if (f == NULL) {
        fprintf(stderr, "cannot open %s\n", argv[1]);
        return 1;
    }
    while (fgets(line, sizeof(line), f) != NULL)
        printf("%4d  %s", ++n, line);
    fclose(f);
    return 0;
}
