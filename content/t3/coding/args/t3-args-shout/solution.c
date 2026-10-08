#include <stdio.h>
#include <ctype.h>

int main(int argc, char *argv[])
{
    for (int i = 1; i < argc; i++) {
        for (char *p = argv[i]; *p; p++)
            putchar(toupper((unsigned char) *p));
        putchar(i < argc - 1 ? ' ' : '\n');
    }
    if (argc == 1)
        putchar('\n');
    return 0;
}
