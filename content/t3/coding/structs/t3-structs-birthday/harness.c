#include <stdio.h>
#include "person.h"

int main(void)
{
    struct person p;

    if (scanf("%31s %d", p.name, &p.age) != 2)
        return 1;
    birthday(&p);
    birthday(&p);
    printf("%s is now %d\n", p.name, p.age);
    return 0;
}
