#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    size_t len = 0, cap = 16;
    char *buf = malloc(cap);
    int c;

    if (buf == NULL)
        return 1;
    while ((c = getchar()) != EOF && c != '\n') {
        if (len + 1 >= cap) {
            char *tmp = realloc(buf, cap * 2);
            if (tmp == NULL) {
                free(buf);
                return 1;
            }
            buf = tmp;
            cap *= 2;
        }
        buf[len++] = (char) c;
    }
    buf[len] = '\0';
    printf("%zu\n", len);
    free(buf);
    return 0;
}
