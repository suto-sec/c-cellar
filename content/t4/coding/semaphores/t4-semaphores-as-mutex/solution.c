#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

static long counter = 0;
static sem_t lock;

static void *add(void *arg)
{
    int i;

    (void)arg;
    for (i = 0; i < 500000; i++) {
        sem_wait(&lock);
        counter++;
        sem_post(&lock);
    }
    return NULL;
}

int main(void)
{
    pthread_t a, b;

    sem_init(&lock, 0, 1);
    pthread_create(&a, NULL, add, NULL);
    pthread_create(&b, NULL, add, NULL);
    pthread_join(a, NULL);
    pthread_join(b, NULL);
    sem_destroy(&lock);
    printf("counter = %ld\n", counter);
    return 0;
}
