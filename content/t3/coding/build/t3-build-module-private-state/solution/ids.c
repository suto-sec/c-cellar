#include "ids.h"

static int last = 0;

int next_id(void)
{
    return ++last;
}
