#include <stdio.h>
#include "counter.h"
#include "counter.h"

int main(void)
{
    struct counter c = {0};

    counter_inc(&c);
    counter_inc(&c);
    counter_inc(&c);
    printf("counter=%d\n", counter_get(&c));
    return 0;
}
