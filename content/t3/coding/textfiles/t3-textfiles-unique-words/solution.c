#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

static int compare(const void *a, const void *b)
{
    return strcmp(*(char * const *) a, *(char * const *) b);
}

int main(int argc, char *argv[])
{
    FILE *f;
    char w[64];
    char **words = NULL;
    size_t n = 0, cap = 0;

    if (argc != 2) {
        fprintf(stderr, "usage: uniquewords FILE\n");
        return 2;
    }
    if ((f = fopen(argv[1], "r")) == NULL) {
        fprintf(stderr, "uniquewords: %s: %s\n", argv[1], strerror(errno));
        return 1;
    }
    while (fscanf(f, "%63s", w) == 1) {
        if (n == cap) {
            cap = cap ? cap * 2 : 32;
            words = realloc(words, cap * sizeof(char *));
            if (words == NULL)
                return 1;
        }
        words[n++] = strdup(w);
    }
    fclose(f);
    if (n > 1)
        qsort(words, n, sizeof(char *), compare);
    for (size_t i = 0; i < n; i++)
        if (i == 0 || strcmp(words[i], words[i - 1]) != 0)
            puts(words[i]);
    for (size_t i = 0; i < n; i++)
        free(words[i]);
    free(words);
    return 0;
}
