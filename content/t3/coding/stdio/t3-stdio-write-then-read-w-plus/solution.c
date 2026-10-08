#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int n, x;
    long sum = 0;

    if (argc != 3)
        return 1;
    n = atoi(argv[2]);
    FILE *f = fopen(argv[1], "w+");
    if (f == NULL)
        return 1;
    for (int i = 1; i <= n; i++)
        fprintf(f, "%d\n", i * i);
    rewind(f);
    while (fscanf(f, "%d", &x) == 1)
        sum += x;
    fclose(f);
    printf("sum = %ld\n", sum);
    return 0;
}
