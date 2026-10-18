#include <stdio.h>

int main(void)
{
    FILE *f = fopen("numbers.txt", "w");

    for (int i = 1; i <= 5; i++)
        fprintf(f, "%d\n", i);
    fclose(f);
    return 0;
}
