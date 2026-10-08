#ifndef COUNTER_H
#define COUNTER_H

struct counter {
    int value;
};

void counter_inc(struct counter *c);
int counter_get(const struct counter *c);

#endif
