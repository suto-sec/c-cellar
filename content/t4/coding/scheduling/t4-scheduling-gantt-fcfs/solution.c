#include <stdio.h>

#define MAX 50

int main(void)
{
    int n, i, j, t = 0, arr[MAX], dur[MAX], order[MAX];

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

        if (arr[p] > t) {
            printf("%d-%d idle\n", t, arr[p]);
            t = arr[p];
        }
        printf("%d-%d P%d\n", t, t + dur[p], p + 1);
        t += dur[p];
    }
    return 0;
}
