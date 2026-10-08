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
    double sum = 0;
    int count = 0;

    for (int i = 0; i < n; i++)
        sum += v[i];
    for (int i = 0; i < n; i++)
        if (v[i] > sum / n)
            count++;
    printf("%d\n", count);
    return 0;
}
