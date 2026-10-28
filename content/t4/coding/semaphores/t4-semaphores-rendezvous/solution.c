#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

static sem_t a_arrived, b_arrived;

static void *thread_a(void *arg)
{
    (void)arg;
    printf("A before\n");
    sem_post(&a_arrived);
    sem_wait(&b_arrived);
    printf("A after\n");
    return NULL;
}

static void *thread_b(void *arg)
{
    (void)arg;
    printf("B before\n");
    sem_post(&b_arrived);
    sem_wait(&a_arrived);
    printf("B after\n");
    return NULL;
}

int main(void)
{
    pthread_t a, b;

    sem_init(&a_arrived, 0, 0);
    sem_init(&b_arrived, 0, 0);
    pthread_create(&a, NULL, thread_a, NULL);
    pthread_create(&b, NULL, thread_b, NULL);
    pthread_join(a, NULL);
    pthread_join(b, NULL);
    return 0;
}
