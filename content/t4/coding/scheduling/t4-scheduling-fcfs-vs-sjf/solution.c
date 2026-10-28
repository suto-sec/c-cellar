#include <stdio.h>

#define MAX 50

static double average_wait(const int *d, int n)
{
    long wait = 0, total = 0;
    int i;

    for (i = 0; i < n; i++) {
        total += wait;
        wait += d[i];
    }
    return (double)total / n;
}

int main(void)
{
    int n, i, j, d[MAX], s[MAX];

    if (scanf("%d", &n) != 1 || n < 1 || n > MAX)
        return 1;
    for (i = 0; i < n; i++) {
        if (scanf("%d", &d[i]) != 1)
            return 1;
        s[i] = d[i];
    }
    for (i = 1; i < n; i++) {
        int v = s[i];

        for (j = i - 1; j >= 0 && s[j] > v; j--)
            s[j + 1] = s[j];
        s[j + 1] = v;
    }
    printf("FCFS: %.2f\nSJF: %.2f\n", average_wait(d, n), average_wait(s, n));
    return 0;
}
