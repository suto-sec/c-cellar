#include <stdio.h>

void print_binary(unsigned n)
{
    if (n > 1)
        print_binary(n / 2);
    putchar('0' + n % 2);
}
