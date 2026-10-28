#include <stdio.h>
#include <pthread.h>

static long acc[2] = {1000, 1000};
static pthread_mutex_t lock[2] = {PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER};

static void transfer(int from, int to)
{
    int first = from < to ? from : to;
    int second = from < to ? to : from;

    pthread_mutex_lock(&lock[first]);
    pthread_mutex_lock(&lock[second]);
    acc[from]--;
    acc[to]++;
    pthread_mutex_unlock(&lock[second]);
    pthread_mutex_unlock(&lock[first]);
}

static void *zero_to_one(void *arg)
{
    int i;

    (void)arg;
    for (i = 0; i < 5000; i++)
        transfer(0, 1);
    return NULL;
}

static void *one_to_zero(void *arg)
{
    int i;

    (void)arg;
    for (i = 0; i < 5000; i++)
        transfer(1, 0);
    return NULL;
}

int main(void)
{
    pthread_t t1, t2;

    pthread_create(&t1, NULL, zero_to_one, NULL);
    pthread_create(&t2, NULL, one_to_zero, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    printf("a=%ld b=%ld total=%ld\n", acc[0], acc[1], acc[0] + acc[1]);
    return 0;
}
