#include <stdio.h>

#define MAX 50

int main(void)
{
    int n, i, best, t = 0, finished = 0, arr[MAX], dur[MAX], rem[MAX], res[MAX];
    long total = 0;

    if (scanf("%d", &n) != 1 || n < 1 || n > MAX)
        return 1;
    for (i = 0; i < n; i++)
        if (scanf("%d %d", &arr[i], &dur[i]) != 2)
            return 1;
    for (i = 0; i < n; i++)
        rem[i] = dur[i];
    while (finished < n) {
        best = -1;
        for (i = 0; i < n; i++)
            if (arr[i] <= t && rem[i] > 0 &&
                (best < 0 || rem[i] < rem[best] || (rem[i] == rem[best] && arr[i] < arr[best])))
                best = i;
        if (best < 0) {
            t++;
            continue;
        }
        rem[best]--;
        t++;
        if (rem[best] == 0) {
            res[best] = t - arr[best] - dur[best];
            finished++;
        }
    }

    for (i = 0; i < n; i++) {
        printf("P%d: %d\n", i + 1, res[i]);
        total += res[i];
    }
    printf("average: %.2f\n", (double)total / n);
    return 0;
}
