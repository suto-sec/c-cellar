#include <stdio.h>
#include <pthread.h>

#define WORKERS 5

static pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t go_cv = PTHREAD_COND_INITIALIZER;
static pthread_cond_t all_waiting_cv = PTHREAD_COND_INITIALIZER;
static int waiting = 0, go = 0, started = 0;

static void *worker(void *arg)
{
    (void)arg;
    pthread_mutex_lock(&m);
    waiting++;
    pthread_cond_signal(&all_waiting_cv);
    while (!go)
        pthread_cond_wait(&go_cv, &m);
    started++;
    pthread_mutex_unlock(&m);
    return NULL;
}

int main(void)
{
    pthread_t tid[WORKERS];
    int i;

    for (i = 0; i < WORKERS; i++)
        pthread_create(&tid[i], NULL, worker, NULL);
    pthread_mutex_lock(&m);
    while (waiting < WORKERS)
        pthread_cond_wait(&all_waiting_cv, &m);
    go = 1;
    pthread_cond_broadcast(&go_cv);
    pthread_mutex_unlock(&m);
    for (i = 0; i < WORKERS; i++)
        pthread_join(tid[i], NULL);
    printf("workers started: %d\n", started);
    return 0;
}
