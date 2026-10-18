#include <stdio.h>
#include <ctype.h>
#include <string.h>

static int blank(const char *s)
{
    for (; *s; s++)
        if (!isspace((unsigned char) *s))
            return 0;
    return 1;
}

struct fraction {
    long num, den;
};

static long gcd(long a, long b)
{
    if (a < 0)
        a = -a;
    if (b < 0)
        b = -b;
    while (b != 0) {
        long t = a % b;
        a = b;
        b = t;
    }
    return a;
}

static struct fraction reduce(struct fraction f)
{
    long g = gcd(f.num, f.den);

    if (g > 1) {
        f.num /= g;
        f.den /= g;
    }
    if (f.den < 0) {
        f.num = -f.num;
        f.den = -f.den;
    }
    return f;
}

static struct fraction add(struct fraction a, struct fraction b)
{
    struct fraction r = { a.num * b.den + b.num * a.den, a.den * b.den };
    return reduce(r);
}

static struct fraction sub(struct fraction a, struct fraction b)
{
    struct fraction r = { a.num * b.den - b.num * a.den, a.den * b.den };
    return reduce(r);
}

static struct fraction mul(struct fraction a, struct fraction b)
{
    struct fraction r = { a.num * b.num, a.den * b.den };
    return reduce(r);
}

static struct fraction divide(struct fraction a, struct fraction b)
{
    struct fraction r = { a.num * b.den, a.den * b.num };
    return reduce(r);
}

int main(void)
{
    char line[256], op;
    struct fraction a, b, r;

    while (fgets(line, sizeof(line), stdin) != NULL) {
        if (blank(line))
            continue;
        if (sscanf(line, "%ld/%ld %c %ld/%ld", &a.num, &a.den, &op, &b.num, &b.den) != 5
            || a.den == 0 || b.den == 0 || strchr("+-*/", op) == NULL || (op == '/' && b.num == 0)) {
            printf("error\n");
            continue;
        }
        r = op == '+' ? add(a, b) : op == '-' ? sub(a, b) : op == '*' ? mul(a, b) : divide(a, b);
        if (r.den == 1)
            printf("%ld\n", r.num);
        else
            printf("%ld/%ld\n", r.num, r.den);
    }
    return 0;
}
