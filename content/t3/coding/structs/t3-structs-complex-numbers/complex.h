#ifndef COMPLEX_H
#define COMPLEX_H

struct complex {
    double re, im;
};

struct complex add(struct complex a, struct complex b);
struct complex mul(struct complex a, struct complex b);

#endif
