#include <stdio.h>

int main(int argc, char *argv[])
{
    FILE *f;
    int x;
    long sum = 0;

    if (argc != 2)
        return 2;
    f = fopen(argv[1], "r");
    if (f == NULL) {
        fprintf(stderr, "cannot open %s\n", argv[1]);
        return 1;
    }
    while (fscanf(f, "%d", &x) == 1)
        sum += x;
    fclose(f);
    printf("%ld\n", sum);
    return 0;
}
