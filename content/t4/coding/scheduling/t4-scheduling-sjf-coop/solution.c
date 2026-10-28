#include <stdio.h>

#define MAX 50

int main(void)
{
    int n, i, k, best, next, t = 0, arr[MAX], dur[MAX], res[MAX], done[MAX] = {0};
    long total = 0;

    if (scanf("%d", &n) != 1 || n < 1 || n > MAX)
        return 1;
    for (i = 0; i < n; i++)
        if (scanf("%d %d", &arr[i], &dur[i]) != 2)
            return 1;
    for (k = 0; k < n; k++) {
        best = -1;
        for (;;) {
            for (i = 0; i < n; i++)
                if (!done[i] && arr[i] <= t &&
                    (best < 0 || dur[i] < dur[best] || (dur[i] == dur[best] && arr[i] < arr[best])))
                    best = i;
            if (best >= 0)
                break;
            next = -1;
            for (i = 0; i < n; i++)
                if (!done[i] && (next < 0 || arr[i] < next))
                    next = arr[i];
            t = next;
        }
        res[best] = t - arr[best];
        t += dur[best];
        done[best] = 1;
    }

    for (i = 0; i < n; i++) {
        printf("P%d: %d\n", i + 1, res[i]);
        total += res[i];
    }
    printf("average: %.2f\n", (double)total / n);
    return 0;
}
