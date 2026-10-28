#include <stdio.h>
#include <pthread.h>

struct part {
    const int *data;
    int from, to;
    long sum;
};

static void *add_part(void *arg)
{
    struct part *p = arg;
    int i;

    p->sum = 0;
    for (i = p->from; i < p->to; i++)
        p->sum += p->data[i];
    return NULL;
}

int main(void)
{
    int data[1000], n, i;
    pthread_t t1, t2;
    struct part a, b;

    if (scanf("%d", &n) != 1 || n < 0 || n > 1000)
        return 1;
    for (i = 0; i < n; i++)
        if (scanf("%d", &data[i]) != 1)
            return 1;
    a = (struct part){data, 0, n / 2, 0};
    b = (struct part){data, n / 2, n, 0};
    pthread_create(&t1, NULL, add_part, &a);
    pthread_create(&t2, NULL, add_part, &b);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    printf("total: %ld\n", a.sum + b.sum);
    return 0;
}
