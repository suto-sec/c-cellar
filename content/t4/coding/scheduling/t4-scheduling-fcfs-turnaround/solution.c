#include <stdio.h>

#define MAX 50

int main(void)
{
    int n, i, j, arr[MAX], dur[MAX], order[MAX], res[MAX];
    int t = 0;
    long total = 0;

    if (scanf("%d", &n) != 1 || n < 1 || n > MAX)
        return 1;
    for (i = 0; i < n; i++)
        if (scanf("%d %d", &arr[i], &dur[i]) != 2)
            return 1;
    for (i = 0; i < n; i++)
        order[i] = i;
    for (i = 1; i < n; i++) {
        int p = order[i];

        for (j = i - 1; j >= 0 && arr[order[j]] > arr[p]; j--)
            order[j + 1] = order[j];
        order[j + 1] = p;
    }
    for (i = 0; i < n; i++) {
        int p = order[i];
        int start = t > arr[p] ? t : arr[p];

        t = start + dur[p];
        res[p] = t - arr[p];
    }

    for (i = 0; i < n; i++) {
        printf("P%d: %d\n", i + 1, res[i]);
        total += res[i];
    }
    printf("average: %.2f\n", (double)total / n);
    return 0;
}
