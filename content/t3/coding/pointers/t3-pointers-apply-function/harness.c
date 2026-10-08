#include <stdio.h>
#include <string.h>

void apply(int *v, int n, int (*f)(int));

static int twice(int x) { return 2 * x; }
static int neg(int x) { return -x; }

int main(void)
{
    int n, v[100];
    char op[16];

    if (scanf("%d", &n) != 1 || n < 1 || n > 100)
        return 1;
    for (int i = 0; i < n; i++)
        if (scanf("%d", &v[i]) != 1)
            return 1;
    if (scanf("%15s", op) != 1)
        return 1;
    apply(v, n, strcmp(op, "double") == 0 ? twice : neg);
    for (int i = 0; i < n; i++)
        printf("%d%s", v[i], i < n - 1 ? " " : "\n");
    return 0;
}
