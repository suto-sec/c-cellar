#include <stdio.h>

int main(void)
{
    unsigned n;

    if (scanf("%u", &n) != 1)
        return 1;
    printf("and=%u\n", n & 0x0F);
    printf("or=%u\n", n | 0x80);
    printf("xor=%u\n", n ^ 0xFF);
    printf("shl=%u\n", n << 2);
    printf("shr=%u\n", n >> 3);
    return 0;
}
