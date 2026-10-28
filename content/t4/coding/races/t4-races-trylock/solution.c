#include <stdio.h>
#include <errno.h>
#include <pthread.h>

static pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

static void *probe(void *arg)
{
    int r = pthread_mutex_trylock(&m);

    (void)arg;
    if (r == EBUSY) {
        printf("trylock while locked: busy\n");
    } else {
        printf("trylock while locked: ok\n");
        pthread_mutex_unlock(&m);
    }
    return NULL;
}

int main(void)
{
    pthread_t tid;
    int r;

    pthread_mutex_lock(&m);
    pthread_create(&tid, NULL, probe, NULL);
    pthread_join(tid, NULL);
    pthread_mutex_unlock(&m);
    r = pthread_mutex_trylock(&m);
    if (r == 0) {
        printf("trylock after unlock: ok\n");
        pthread_mutex_unlock(&m);
    } else {
        printf("trylock after unlock: busy\n");
    }
    return 0;
}
