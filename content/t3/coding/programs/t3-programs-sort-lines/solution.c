#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int cmp(const void *a, const void *b)
{
    return strcmp(*(char * const *) a, *(char * const *) b);
}

int main(int argc, char *argv[])
{
    FILE *f;
    char line[1024], **v = NULL;
    size_t n = 0, cap = 0;

    if (argc != 2)
        return 2;
    f = fopen(argv[1], "r");
    if (f == NULL) {
        fprintf(stderr, "cannot open %s\n", argv[1]);
        return 1;
    }
    while (fgets(line, sizeof(line), f) != NULL) {
        line[strcspn(line, "\n")] = '\0';
        if (n == cap) {
            cap = cap ? cap * 2 : 16;
            v = realloc(v, cap * sizeof(char *));
            if (v == NULL)
                return 1;
        }
        v[n++] = strdup(line);
    }
    fclose(f);
    qsort(v, n, sizeof(char *), cmp);
    for (size_t i = 0; i < n; i++) {
        puts(v[i]);
        free(v[i]);
    }
    free(v);
    return 0;
}
