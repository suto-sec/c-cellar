#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char *argv[])
{
    long lines = 0, words = 0, bytes = 0;
    int c, inword = 0;
    char mode = 0;

    if (argc == 2 && strlen(argv[1]) == 2 && argv[1][0] == '-' && strchr("lwc", argv[1][1]))
        mode = argv[1][1];
    else if (argc != 1) {
        fprintf(stderr, "usage: mywc [-l|-w|-c]\n");
        return 2;
    }
    while ((c = getchar()) != EOF) {
        bytes++;
        if (c == '\n')
            lines++;
        if (isspace(c))
            inword = 0;
        else if (!inword) {
            inword = 1;
            words++;
        }
    }
    if (mode == 'l')
        printf("%ld\n", lines);
    else if (mode == 'w')
        printf("%ld\n", words);
    else if (mode == 'c')
        printf("%ld\n", bytes);
    else
        printf("%ld %ld %ld\n", lines, words, bytes);
    return 0;
}
