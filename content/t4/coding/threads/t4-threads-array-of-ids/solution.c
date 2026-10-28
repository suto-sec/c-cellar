#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

static int results[8];

static void *worker(void *arg)
{
    int i = *(int *)arg;

    results[i] = i * i;
    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t tid[8];
    int ids[8], n, i;

    if (argc != 2)
        return 2;
    n = atoi(argv[1]);
    if (n < 1 || n > 8)
        return 2;
    for (i = 0; i < n; i++) {
        ids[i] = i;
        pthread_create(&tid[i], NULL, worker, &ids[i]);
    }
    for (i = 0; i < n; i++)
        pthread_join(tid[i], NULL);
    for (i = 0; i < n; i++)
        printf("%d: %d\n", i, results[i]);
    return 0;
}
