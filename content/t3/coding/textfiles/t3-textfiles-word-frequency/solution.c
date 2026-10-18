#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

struct entry {
    char word[64];
    int count;
};

int main(int argc, char *argv[])
{
    FILE *f;
    char w[64];
    struct entry *list = NULL;
    size_t n = 0, cap = 0;

    if (argc != 2) {
        fprintf(stderr, "usage: wordfreq FILE\n");
        return 2;
    }
    if ((f = fopen(argv[1], "r")) == NULL) {
        fprintf(stderr, "wordfreq: %s: %s\n", argv[1], strerror(errno));
        return 1;
    }
    while (fscanf(f, "%63s", w) == 1) {
        size_t i;
        for (i = 0; i < n; i++)
            if (strcmp(list[i].word, w) == 0)
                break;
        if (i < n) {
            list[i].count++;
            continue;
        }
        if (n == cap) {
            cap = cap ? cap * 2 : 16;
            list = realloc(list, cap * sizeof(struct entry));
            if (list == NULL)
                return 1;
        }
        strcpy(list[n].word, w);
        list[n].count = 1;
        n++;
    }
    fclose(f);
    for (size_t i = 0; i < n; i++)
        printf("%s: %d\n", list[i].word, list[i].count);
    free(list);
    return 0;
}
