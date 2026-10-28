#include <stdio.h>
#include <pthread.h>

static int value = 1;

static void *worker(void *arg)
{
    (void)arg;
    value = 42;
    return NULL;
}

int main(void)
{
    pthread_t tid;

    printf("before: %d\n", value);
    pthread_create(&tid, NULL, worker, NULL);
    pthread_join(tid, NULL);
    printf("after: %d\n", value);
    return 0;
}
