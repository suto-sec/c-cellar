#include <stdio.h>

int main(void)
{
    FILE *f = fopen("hello.txt", "w");

    fputs("Hello, file!\n", f);
    fclose(f);
    return 0;
}
