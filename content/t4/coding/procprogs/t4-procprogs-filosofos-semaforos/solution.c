#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define N 5

static sem_t chopstick[N];
static int meals[N];

static void *philosopher(void *arg)
{
    int i = *(int *)arg;
    int left = i, right = (i + 1) % N;
    int first = left < right ? left : right;
    int second = left < right ? right : left;
    int round;

    for (round = 0; round < 3; round++) {
        sem_wait(&chopstick[first]);
        sem_wait(&chopstick[second]);
        meals[i]++;
        sem_post(&chopstick[second]);
        sem_post(&chopstick[first]);
    }
    return NULL;
}

int main(void)
{
    pthread_t tid[N];
    int ids[N], i, total = 0;

    for (i = 0; i < N; i++)
        sem_init(&chopstick[i], 0, 1);
    for (i = 0; i < N; i++) {
        ids[i] = i;
        pthread_create(&tid[i], NULL, philosopher, &ids[i]);
    }
    for (i = 0; i < N; i++)
        pthread_join(tid[i], NULL);
    for (i = 0; i < N; i++) {
        printf("philosopher %d ate %d times\n", i, meals[i]);
        total += meals[i];
        sem_destroy(&chopstick[i]);
    }
    printf("total meals: %d\n", total);
    return 0;
}
