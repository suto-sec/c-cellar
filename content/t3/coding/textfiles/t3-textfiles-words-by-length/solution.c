#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main(int argc, char *argv[])
{
    FILE *f;
    char w[101];
    char **words = NULL;
    size_t n = 0, cap = 0, maxlen = 0;

    if (argc != 2) {
        fprintf(stderr, "usage: bylength FILE\n");
        return 2;
    }
    if ((f = fopen(argv[1], "r")) == NULL) {
        fprintf(stderr, "bylength: %s: %s\n", argv[1], strerror(errno));
        return 1;
    }
    while (fscanf(f, "%100s", w) == 1) {
        if (n == cap) {
            cap = cap ? cap * 2 : 32;
            words = realloc(words, cap * sizeof(char *));
            if (words == NULL)
                return 1;
        }
        words[n] = strdup(w);
        if (strlen(w) > maxlen)
            maxlen = strlen(w);
        n++;
    }
    fclose(f);
    for (size_t len = 1; len <= maxlen; len++) {
        int first = 1;
        for (size_t i = 0; i < n; i++) {
            if (strlen(words[i]) != len)
                continue;
            if (first)
                printf("%zu:", len);
            printf(" %s", words[i]);
            first = 0;
        }
        if (!first)
            putchar('\n');
    }
    for (size_t i = 0; i < n; i++)
        free(words[i]);
    free(words);
    return 0;
}
