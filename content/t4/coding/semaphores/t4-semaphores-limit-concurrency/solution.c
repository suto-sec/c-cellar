#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

static sem_t room;
static pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
static int inside = 0, max_inside = 0;

static void *visitor(void *arg)
{
    (void)arg;
    sem_wait(&room);
    pthread_mutex_lock(&m);
    inside++;
    if (inside > max_inside)
        max_inside = inside;
    pthread_mutex_unlock(&m);
    sleep(1);
    pthread_mutex_lock(&m);
    inside--;
    pthread_mutex_unlock(&m);
    sem_post(&room);
    return NULL;
}

int main(void)
{
    pthread_t tid[6];
    int i;

    sem_init(&room, 0, 2);
    for (i = 0; i < 6; i++)
        pthread_create(&tid[i], NULL, visitor, NULL);
    for (i = 0; i < 6; i++)
        pthread_join(tid[i], NULL);
    sem_destroy(&room);
    printf("max inside: %d\n", max_inside);
    return 0;
}
