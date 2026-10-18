#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    char word[201];
    int width;
    size_t col = 0;

    if (argc != 2 || (width = atoi(argv[1])) <= 0) {
        fprintf(stderr, "usage: wrap WIDTH\n");
        return 2;
    }
    while (scanf("%200s", word) == 1) {
        size_t len = strlen(word);
        if (col == 0) {
            fputs(word, stdout);
            col = len;
        } else if (col + 1 + len <= (size_t) width) {
            printf(" %s", word);
            col += 1 + len;
        } else {
            printf("\n%s", word);
            col = len;
        }
    }
    if (col > 0)
        putchar('\n');
    return 0;
}
