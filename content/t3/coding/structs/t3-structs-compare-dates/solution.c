#include <stdio.h>

struct date {
    int day, month, year;
};

static int compare(struct date a, struct date b)
{
    if (a.year != b.year)
        return a.year - b.year;
    if (a.month != b.month)
        return a.month - b.month;
    return a.day - b.day;
}

int main(void)
{
    struct date a, b;
    int c;

    if (scanf("%d %d %d %d %d %d", &a.day, &a.month, &a.year, &b.day, &b.month, &b.year) != 6)
        return 1;
    c = compare(a, b);
    printf("%s\n", c == 0 ? "same" : c < 0 ? "first is earlier" : "first is later");
    return 0;
}
