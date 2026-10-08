#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct student {
    char name[32];
    int score;
};

static int cmp(const void *a, const void *b)
{
    const struct student *x = a, *y = b;

    if (x->score != y->score)
        return y->score - x->score;
    return strcmp(x->name, y->name);
}

int main(void)
{
    struct student v[50];
    int n;

    if (scanf("%d", &n) != 1 || n < 1 || n > 50)
        return 1;
    for (int i = 0; i < n; i++)
        if (scanf("%31s %d", v[i].name, &v[i].score) != 2)
            return 1;
    qsort(v, (size_t) n, sizeof(struct student), cmp);
    for (int i = 0; i < n; i++)
        printf("%s: %d\n", v[i].name, v[i].score);
    return 0;
}
