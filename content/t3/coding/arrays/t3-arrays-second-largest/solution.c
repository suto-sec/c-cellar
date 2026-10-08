#include <stdio.h>

#define MAX 100

int main(void)
{
    int n, v[MAX];

    if (scanf("%d", &n) != 1 || n < 1 || n > MAX)
        return 1;
    for (int i = 0; i < n; i++)
        if (scanf("%d", &v[i]) != 1)
            return 1;
    int first = v[0], second = 0, has_second = 0;

    for (int i = 1; i < n; i++) {
        if (v[i] > first) {
            second = first;
            has_second = 1;
            first = v[i];
        } else if (v[i] < first && (!has_second || v[i] > second)) {
            second = v[i];
            has_second = 1;
        }
    }
    if (has_second)
        printf("%d\n", second);
    else
        printf("none\n");
    return 0;
}
