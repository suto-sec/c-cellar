#include <stdio.h>

int main(void)
{
    int n;
    long long a = 0, b = 1, t;

    if (scanf("%d", &n) != 1)
        return 1;
    for (int i = 0; i < n; i++) {
        printf("%s%lld", i ? " " : "", a);
        t = a + b;
        a = b;
        b = t;
    }
    printf("\n");
    return 0;
}
