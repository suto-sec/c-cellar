#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int reverse, numeric;

static int compare(const void *pa, const void *pb)
{
    const char *a = *(char * const *) pa, *b = *(char * const *) pb;
    int r = 0;

    if (numeric) {
        long x = strtol(a, NULL, 10), y = strtol(b, NULL, 10);
        r = (x > y) - (x < y);
    }
    if (r == 0)
        r = strcmp(a, b);
    return reverse ? -r : r;
}

int main(int argc, char *argv[])
{
    char buf[1024];
    char **lines = NULL;
    size_t n = 0, cap = 0;
    int unique = 0;

    for (int i = 1; i < argc; i++) {
        if (argv[i][0] != '-' || argv[i][1] == '\0') {
            fprintf(stderr, "usage: mysort [-r] [-n] [-u]\n");
            return 2;
        }
        for (const char *p = argv[i] + 1; *p; p++) {
            if (*p == 'r')
                reverse = 1;
            else if (*p == 'n')
                numeric = 1;
            else if (*p == 'u')
                unique = 1;
            else {
                fprintf(stderr, "mysort: invalid option -- '%c'\n", *p);
                return 2;
            }
        }
    }
    while (fgets(buf, sizeof(buf), stdin) != NULL) {
        if (n == cap) {
            cap = cap ? cap * 2 : 16;
            lines = realloc(lines, cap * sizeof(char *));
            if (lines == NULL)
                return 1;
        }
        lines[n++] = strdup(buf);
    }
    if (n > 1)
        qsort(lines, n, sizeof(char *), compare);
    for (size_t i = 0; i < n; i++)
        if (!unique || i == 0 || strcmp(lines[i], lines[i - 1]) != 0)
            fputs(lines[i], stdout);
    for (size_t i = 0; i < n; i++)
        free(lines[i]);
    free(lines);
    return 0;
}
