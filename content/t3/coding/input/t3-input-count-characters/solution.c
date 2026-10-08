#include <stdio.h>

int main(void)
{
    int c;
    long chars = 0, lines = 0;

    while ((c = getchar()) != EOF) {
        chars++;
        if (c == '\n')
            lines++;
    }
    printf("chars=%ld lines=%ld\n", chars, lines);
    return 0;
}
