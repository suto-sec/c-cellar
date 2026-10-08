#include <stdio.h>
#include <string.h>

int main(void)
{
    char w[128];
    size_t n;

    if (scanf("%127s", w) != 1)
        return 1;
    n = strlen(w);
    for (size_t i = 0; i < n / 2; i++) {
        char t = w[i];
        w[i] = w[n - 1 - i];
        w[n - 1 - i] = t;
    }
    printf("%s\n", w);
    return 0;
}
