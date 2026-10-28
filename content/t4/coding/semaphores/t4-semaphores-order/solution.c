#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

static sem_t done_first;

static void *print_first(void *arg)
{
    (void)arg;
    printf("first\n");
    sem_post(&done_first);
    return NULL;
}

static void *print_second(void *arg)
{
    (void)arg;
    sem_wait(&done_first);
    printf("second\n");
    return NULL;
}

int main(void)
{
    pthread_t a, b;

    sem_init(&done_first, 0, 0);
    pthread_create(&b, NULL, print_second, NULL);
    pthread_create(&a, NULL, print_first, NULL);
    pthread_join(a, NULL);
    pthread_join(b, NULL);
    sem_destroy(&done_first);
    return 0;
}
