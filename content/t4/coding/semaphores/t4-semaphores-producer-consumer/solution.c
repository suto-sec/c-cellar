#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define SLOTS 4

static int buffer[SLOTS];
static int in = 0, out = 0;
static sem_t empty, full, mutex;
static int total;
static long sum = 0;

static void *producer(void *arg)
{
    int i;

    (void)arg;
    for (i = 1; i <= total; i++) {
        sem_wait(&empty);
        sem_wait(&mutex);
        buffer[in] = i;
        in = (in + 1) % SLOTS;
        sem_post(&mutex);
        sem_post(&full);
    }
    return NULL;
}

static void *consumer(void *arg)
{
    int i, v;

    (void)arg;
    for (i = 0; i < total; i++) {
        sem_wait(&full);
        sem_wait(&mutex);
        v = buffer[out];
        out = (out + 1) % SLOTS;
        sem_post(&mutex);
        sem_post(&empty);
        sum += v;
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t p, c;

    if (argc != 2)
        return 2;
    total = atoi(argv[1]);
    sem_init(&empty, 0, SLOTS);
    sem_init(&full, 0, 0);
    sem_init(&mutex, 0, 1);
    pthread_create(&p, NULL, producer, NULL);
    pthread_create(&c, NULL, consumer, NULL);
    pthread_join(p, NULL);
    pthread_join(c, NULL);
    printf("sum = %ld\n", sum);
    return 0;
}
