#include <stdio.h>

int main(int argc, char *argv[])
{
    int x;
    long sum = 0;
    int count = 0;

    if (argc != 2)
        return 1;
    FILE *f = fopen(argv[1], "r");
    if (f == NULL)
        return 1;
    while (fscanf(f, "%d", &x) == 1) {
        sum += x;
        count++;
    }
    if (count == 0)
        return 1;
    double average = (double)sum / count;

    printf("average = %.2f\n", average);
    rewind(f);
    while (fscanf(f, "%d", &x) == 1)
        if (x > average)
            printf("%d\n", x);
    fclose(f);
    return 0;
}
