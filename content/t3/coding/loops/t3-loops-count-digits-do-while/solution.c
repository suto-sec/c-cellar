#include <stdio.h>

int main(void)
{
    int n;

    while (scanf("%d", &n) == 1) {
        int count = 0;
        long rest = n < 0 ? -(long)n : n;

        do {
            count++;
            rest /= 10;
        } while (rest != 0);
        printf("%d has %d digit%s\n", n, count, count == 1 ? "" : "s");
    }
    return 0;
}
