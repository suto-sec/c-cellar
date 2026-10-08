#include "complex.h"

struct complex add(struct complex a, struct complex b)
{
    struct complex r = {a.re + b.re, a.im + b.im};
    return r;
}

struct complex mul(struct complex a, struct complex b)
{
    struct complex r = {a.re * b.re - a.im * b.im, a.re * b.im + a.im * b.re};
    return r;
}
