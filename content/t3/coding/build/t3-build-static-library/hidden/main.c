#include <stdio.h>
#include "strutil.h"

int main(void)
{
    const char *s = "banana bread";

    printf("a=%d z=%d space=%d\n", count_char(s, 'a'), count_char(s, 'z'), count_char(s, ' ') + 1);
    return 0;
}
