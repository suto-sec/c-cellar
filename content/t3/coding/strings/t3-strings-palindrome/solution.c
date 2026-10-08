#include <stdio.h>
#include <string.h>

int main(void)
{
    char w[128];
    size_t n;
    int ok = 1;

    if (scanf("%127s", w) != 1)
        return 1;
    n = strlen(w);
    for (size_t i = 0; i < n / 2; i++)
        if (w[i] != w[n - 1 - i])
            ok = 0;
    printf("%s\n", ok ? "yes" : "no");
    return 0;
}
