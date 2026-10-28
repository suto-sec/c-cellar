#include <stdio.h>
#include <pthread.h>

static pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t c = PTHREAD_COND_INITIALIZER;
static int ready = 0;

static void *worker(void *arg)
{
    (void)arg;
    pthread_mutex_lock(&m);
    printf("worker: ready\n");
    ready = 1;
    pthread_cond_signal(&c);
    pthread_mutex_unlock(&m);
    return NULL;
}

int main(void)
{
    pthread_t tid;

    pthread_create(&tid, NULL, worker, NULL);
    pthread_mutex_lock(&m);
    while (!ready)
        pthread_cond_wait(&c, &m);
    pthread_mutex_unlock(&m);
    printf("main: go\n");
    pthread_join(tid, NULL);
    return 0;
}
