#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct entry {
    char word[64];
    int count;
};

static struct entry *table;
static size_t n, cap;

static void add(const char *w)
{
    for (size_t i = 0; i < n; i++)
        if (strcmp(table[i].word, w) == 0) {
            table[i].count++;
            return;
        }
    if (n == cap) {
        cap = cap ? cap * 2 : 64;
        table = realloc(table, cap * sizeof(struct entry));
        if (table == NULL)
            exit(1);
    }
    strcpy(table[n].word, w);
    table[n++].count = 1;
}

int main(int argc, char *argv[])
{
    FILE *f;
    int c;
    char w[64];
    size_t len = 0, best = 0;

    if (argc != 2)
        return 2;
    f = fopen(argv[1], "r");
    if (f == NULL) {
        fprintf(stderr, "cannot open %s\n", argv[1]);
        return 1;
    }
    while (1) {
        c = fgetc(f);
        if (c != EOF && isalpha(c)) {
            if (len < sizeof(w) - 1)
                w[len++] = (char) tolower(c);
        } else {
            if (len > 0) {
                w[len] = '\0';
                add(w);
                len = 0;
            }
            if (c == EOF)
                break;
        }
    }
    fclose(f);
    if (n == 0) {
        printf("no words\n");
        return 0;
    }
    for (size_t i = 1; i < n; i++)
        if (table[i].count > table[best].count || (table[i].count == table[best].count && strcmp(table[i].word, table[best].word) < 0))
            best = i;
    printf("%s %d\n", table[best].word, table[best].count);
    free(table);
    return 0;
}
