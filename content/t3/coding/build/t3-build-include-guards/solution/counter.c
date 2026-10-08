#include "counter.h"

void counter_inc(struct counter *c)
{
    c->value++;
}

int counter_get(const struct counter *c)
{
    return c->value;
}
