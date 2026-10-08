#include <stdio.h>

int main(int argc, char *argv[])
{
    FILE *f;
    char line[256], name[64];
    double price, total = 0;
    int items = 0;

    if (argc != 2)
        return 2;
    f = fopen(argv[1], "r");
    if (f == NULL) {
        fprintf(stderr, "cannot open %s\n", argv[1]);
        return 1;
    }
    while (fgets(line, sizeof(line), f) != NULL)
        if (sscanf(line, "%63[^,],%lf", name, &price) == 2) {
            total += price;
            items++;
        }
    fclose(f);
    printf("items=%d total=%.2f\n", items, total);
    return 0;
}
