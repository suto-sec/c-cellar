#include <stdio.h>
#include <pthread.h>

#define N 5

enum state { THINKING, HUNGRY, EATING };

static enum state st[N];
static int meals[N];
static pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cv = PTHREAD_COND_INITIALIZER;

static void *philosopher(void *arg)
{
    int i = *(int *)arg;
    int left = (i + N - 1) % N, right = (i + 1) % N;
    int round;

    for (round = 0; round < 3; round++) {
        pthread_mutex_lock(&m);
        st[i] = HUNGRY;
        while (st[left] == EATING || st[right] == EATING)
            pthread_cond_wait(&cv, &m);
        st[i] = EATING;
        meals[i]++;
        pthread_mutex_unlock(&m);
        pthread_mutex_lock(&m);
        st[i] = THINKING;
        pthread_cond_broadcast(&cv);
        pthread_mutex_unlock(&m);
    }
    return NULL;
}

int main(void)
{
    pthread_t tid[N];
    int ids[N], i, total = 0;

    for (i = 0; i < N; i++) {
        ids[i] = i;
        pthread_create(&tid[i], NULL, philosopher, &ids[i]);
    }
    for (i = 0; i < N; i++)
        pthread_join(tid[i], NULL);
    for (i = 0; i < N; i++) {
        printf("philosopher %d ate %d times\n", i, meals[i]);
        total += meals[i];
    }
    printf("total meals: %d\n", total);
    return 0;
}
