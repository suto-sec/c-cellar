#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

static sem_t can_ping, can_pong;
static int rounds;

static void *pinger(void *arg)
{
    int i;

    (void)arg;
    for (i = 0; i < rounds; i++) {
        sem_wait(&can_ping);
        printf("ping\n");
        sem_post(&can_pong);
    }
    return NULL;
}

static void *ponger(void *arg)
{
    int i;

    (void)arg;
    for (i = 0; i < rounds; i++) {
        sem_wait(&can_pong);
        printf("pong\n");
        sem_post(&can_ping);
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t a, b;

    if (argc != 2)
        return 2;
    rounds = atoi(argv[1]);
    sem_init(&can_ping, 0, 1);
    sem_init(&can_pong, 0, 0);
    pthread_create(&b, NULL, ponger, NULL);
    pthread_create(&a, NULL, pinger, NULL);
    pthread_join(a, NULL);
    pthread_join(b, NULL);
    sem_destroy(&can_ping);
    sem_destroy(&can_pong);
    return 0;
}
