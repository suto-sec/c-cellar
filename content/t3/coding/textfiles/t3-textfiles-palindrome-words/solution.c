#include <stdio.h>
#include <string.h>
#include <errno.h>

static int is_palindrome(const char *s)
{
    size_t i = 0, j = strlen(s);

    if (j < 2)
        return 0;
    for (j--; i < j; i++, j--)
        if (s[i] != s[j])
            return 0;
    return 1;
}

int main(int argc, char *argv[])
{
    FILE *f;
    char w[101];
    int total = 0;

    if (argc != 2) {
        fprintf(stderr, "usage: palwords FILE\n");
        return 2;
    }
    if ((f = fopen(argv[1], "r")) == NULL) {
        fprintf(stderr, "palwords: %s: %s\n", argv[1], strerror(errno));
        return 1;
    }
    while (fscanf(f, "%100s", w) == 1)
        if (is_palindrome(w)) {
            puts(w);
            total++;
        }
    fclose(f);
    printf("total: %d\n", total);
    return 0;
}
