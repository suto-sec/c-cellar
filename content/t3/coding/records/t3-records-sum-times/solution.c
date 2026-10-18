#include <stdio.h>
#include <ctype.h>

static int blank(const char *s)
{
    for (; *s; s++)
        if (!isspace((unsigned char) *s))
            return 0;
    return 1;
}

struct time {
    long h, m, s;
};

static struct time add(struct time a, struct time b)
{
    struct time r = { a.h + b.h, a.m + b.m, a.s + b.s };

    r.m += r.s / 60;
    r.s %= 60;
    r.h += r.m / 60;
    r.m %= 60;
    return r;
}

int main(void)
{
    char line[256];
    struct time total = { 0, 0, 0 }, t;
    int n = 0;

    while (fgets(line, sizeof(line), stdin) != NULL) {
        n++;
        if (blank(line))
            continue;
        if (sscanf(line, "%ld:%ld:%ld", &t.h, &t.m, &t.s) != 3) {
            fprintf(stderr, "sumtimes: line %d: not a time\n", n);
            return 1;
        }
        total = add(total, t);
    }
    printf("%ld:%02ld:%02ld\n", total.h, total.m, total.s);
    return 0;
}
