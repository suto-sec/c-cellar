#include <stdio.h>

int main(int argc, char *argv[])
{
    int value;

    if (argc != 2)
        return 1;
    FILE *f = fopen(argv[1], "r+");
    if (f == NULL)
        return 1;
    if (fscanf(f, "%d", &value) != 1) {
        fclose(f);
        return 1;
    }
    value++;
    rewind(f);
    fprintf(f, "%d\n", value);
    fclose(f);
    printf("%d\n", value);
    return 0;
}
