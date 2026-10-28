#include <stdio.h>

#define MAX 50
#define QUEUE 100000

int main(void)
{
    int n, q, i, t = 0, finished = 0, head = 0, tail = 0, p, run;
    int arr[MAX], dur[MAX], rem[MAX], res[MAX], admitted[MAX] = {0};
    static int queue[QUEUE];
    long total = 0;

    if (scanf("%d %d", &n, &q) != 2 || n < 1 || n > MAX || q < 1)
        return 1;
    for (i = 0; i < n; i++) {
        if (scanf("%d %d", &arr[i], &dur[i]) != 2)
            return 1;
        rem[i] = dur[i];
    }
    while (finished < n) {
        for (;;) {
            int next = -1;

            for (i = 0; i < n; i++)
                if (!admitted[i] && arr[i] <= t && (next < 0 || arr[i] < arr[next]))
                    next = i;
            if (next < 0)
                break;
            admitted[next] = 1;
            queue[tail++ % QUEUE] = next;
        }
        if (head == tail) {
            int soonest = -1;

            for (i = 0; i < n; i++)
                if (!admitted[i] && (soonest < 0 || arr[i] < soonest))
                    soonest = arr[i];
            t = soonest;
            continue;
        }
        p = queue[head++ % QUEUE];
        run = rem[p] < q ? rem[p] : q;
        t += run;
        rem[p] -= run;
        for (;;) {
            int next = -1;

            for (i = 0; i < n; i++)
                if (!admitted[i] && arr[i] <= t && (next < 0 || arr[i] < arr[next]))
                    next = i;
            if (next < 0)
                break;
            admitted[next] = 1;
            queue[tail++ % QUEUE] = next;
        }
        if (rem[p] > 0) {
            queue[tail++ % QUEUE] = p;
        } else {
            res[p] = t - arr[p] - dur[p];
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
