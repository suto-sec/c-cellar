#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

static int blank(const char *s)
{
    for (; *s; s++)
        if (!isspace((unsigned char) *s))
            return 0;
    return 1;
}

struct student {
    char name[32];
    double grade;
};

static int compare(const void *pa, const void *pb)
{
    const struct student *a = pa, *b = pb;

    if (a->grade > b->grade)
        return -1;
    if (a->grade < b->grade)
        return 1;
    return strcmp(a->name, b->name);
}

int main(int argc, char *argv[])
{
    FILE *f;
    char line[1024], extra[2];
    struct student *list = NULL, s;
    size_t n = 0, cap = 0;
    int lineno = 0;
    double sum = 0;

    if (argc != 2) {
        fprintf(stderr, "usage: ranking FILE\n");
        return 2;
    }
    if ((f = fopen(argv[1], "r")) == NULL) {
        fprintf(stderr, "ranking: %s: %s\n", argv[1], strerror(errno));
        return 1;
    }
    while (fgets(line, sizeof(line), f) != NULL) {
        lineno++;
        if (blank(line))
            continue;
        if (sscanf(line, "%31s %lf %1s", s.name, &s.grade, extra) != 2) {
            fprintf(stderr, "ranking: %s: line %d: bad record\n", argv[1], lineno);
            return 1;
        }
        if (n == cap) {
            cap = cap ? cap * 2 : 16;
            list = realloc(list, cap * sizeof(struct student));
            if (list == NULL)
                return 1;
        }
        list[n++] = s;
        sum += s.grade;
    }
    fclose(f);
    if (n == 0) {
        fprintf(stderr, "ranking: %s: no students\n", argv[1]);
        return 1;
    }
    qsort(list, n, sizeof(struct student), compare);
    for (size_t i = 0; i < n; i++)
        printf("%zu. %s %.2f\n", i + 1, list[i].name, list[i].grade);
    printf("average: %.2f\n", sum / (double) n);
    free(list);
    return 0;
}
