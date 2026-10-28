#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

static void *worker(void *arg)
{
    int n = *(int *)arg;

    printf("thread got %d\n", n);
    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t tid;
    int n;

    if (argc != 2)
        return 2;
    n = atoi(argv[1]);
    pthread_create(&tid, NULL, worker, &n);
    pthread_join(tid, NULL);
    return 0;
}
