#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    char word[256], **w = NULL;
    size_t n = 0, cap = 0;

    while (scanf("%255s", word) == 1) {
        if (n == cap) {
            size_t ncap = cap ? cap * 2 : 8;
            char **tmp = realloc(w, ncap * sizeof(char *));
            if (tmp == NULL)
                return 1;
            w = tmp;
            cap = ncap;
        }
        w[n++] = strdup(word);
    }
    for (size_t i = n; i > 0; i--) {
        puts(w[i - 1]);
        free(w[i - 1]);
    }
    free(w);
    return 0;
}
