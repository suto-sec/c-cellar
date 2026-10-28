#include <stdio.h>
#include <pthread.h>

static void *worker(void *arg)
{
    (void)arg;
    printf("hello from thread\n");
    return NULL;
}

int main(void)
{
    pthread_t tid;

    if (pthread_create(&tid, NULL, worker, NULL) != 0)
        return 1;
    pthread_join(tid, NULL);
    printf("joined\n");
    return 0;
}
