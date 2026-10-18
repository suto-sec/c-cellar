#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    char buf[1024];
    char **lines = NULL;
    size_t n = 0, cap = 0;

    while (fgets(buf, sizeof(buf), stdin) != NULL) {
        if (n == cap) {
            char **bigger;
            cap = cap ? cap * 2 : 16;
            bigger = realloc(lines, cap * sizeof(char *));
            if (bigger == NULL)
                return 1;
            lines = bigger;
        }
        lines[n++] = strdup(buf);
    }
    while (n > 0) {
        char *l = lines[--n];
        fputs(l, stdout);
        if (strchr(l, '\n') == NULL)
            putchar('\n');
        free(l);
    }
    free(lines);
    return 0;
}
