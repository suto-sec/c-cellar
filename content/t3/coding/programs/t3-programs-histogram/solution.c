#include <stdio.h>

int main(int argc, char *argv[])
{
    FILE *f;
    int x, count[10] = {0};

    if (argc != 2)
        return 2;
    f = fopen(argv[1], "r");
    if (f == NULL) {
        fprintf(stderr, "cannot open %s\n", argv[1]);
        return 1;
    }
    while (fscanf(f, "%d", &x) == 1)
        if (x >= 0 && x <= 99)
            count[x / 10]++;
    fclose(f);
    for (int b = 0; b < 10; b++) {
        if (count[b] == 0)
            continue;
        printf("%02d-%02d: ", b * 10, b * 10 + 9);
        for (int i = 0; i < count[b]; i++)
            putchar('*');
        putchar('\n');
    }
    return 0;
}
