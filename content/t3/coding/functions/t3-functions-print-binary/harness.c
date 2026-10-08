#include <stdio.h>

void print_binary(unsigned n);

int main(void)
{
    unsigned n;

    while (scanf("%u", &n) == 1) {
        print_binary(n);
        putchar('\n');
    }
    return 0;
}
