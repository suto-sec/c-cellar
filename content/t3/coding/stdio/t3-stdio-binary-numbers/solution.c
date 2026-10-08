#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int n = argc - 1;
    int *v = malloc((size_t) (n > 0 ? n : 1) * sizeof(int));
    long sum = 0;
    FILE *f;

    if (v == NULL)
        return 1;
    for (int i = 0; i < n; i++)
        v[i] = atoi(argv[i + 1]);
    f = fopen("numbers.bin", "wb");
    if (f == NULL)
        return 1;
    fwrite(v, sizeof(int), (size_t) n, f);
    fclose(f);
    f = fopen("numbers.bin", "rb");
    if (f == NULL)
        return 1;
    n = (int) fread(v, sizeof(int), (size_t) n, f);
    fclose(f);
    for (int i = 0; i < n; i++)
        sum += v[i];
    printf("count=%d sum=%ld\n", n, sum);
    free(v);
    return 0;
}
