#include <stdint.h>
#include <stdio.h>

int main(void)
{
    long long input;

    if (scanf("%lld", &input) != 1)
        return 1;
    int64_t n = input;
    uint8_t low = (uint8_t)n;

    printf("square = %lld\n", (long long)(n * n));
    printf("low byte = %d\n", low);
    return 0;
}
