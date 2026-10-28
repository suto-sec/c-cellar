#include <stdio.h>
#include <limits.h>
#include <pthread.h>

struct slice {
    const int *data;
    int from, to;
    int max;
};

static void *find_max(void *arg)
{
    struct slice *s = arg;
    int i;

    s->max = INT_MIN;
    for (i = s->from; i < s->to; i++)
        if (s->data[i] > s->max)
            s->max = s->data[i];
    return NULL;
}

int main(void)
{
    int data[1000], n, i, best;
    pthread_t tid[4];
    struct slice sl[4];

    if (scanf("%d", &n) != 1 || n < 1 || n > 1000)
        return 1;
    for (i = 0; i < n; i++)
        if (scanf("%d", &data[i]) != 1)
            return 1;
    for (i = 0; i < 4; i++) {
        sl[i] = (struct slice){data, i * n / 4, (i + 1) * n / 4, INT_MIN};
        pthread_create(&tid[i], NULL, find_max, &sl[i]);
    }
    best = INT_MIN;
    for (i = 0; i < 4; i++) {
        pthread_join(tid[i], NULL);
        if (sl[i].max > best)
            best = sl[i].max;
    }
    printf("max: %d\n", best);
    return 0;
}
