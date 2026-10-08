#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    for (int i = argc - 1; i >= 1; i--) {
        for (int j = (int)strlen(argv[i]) - 1; j >= 0; j--)
            putchar(argv[i][j]);
        putchar(i > 1 ? ' ' : '\n');
    }
    return 0;
}
