#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

static long counter = 0;
static pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

static void *add(void *arg)
{
    int i;

    (void)arg;
    for (i = 0; i < 100000; i++) {
        pthread_mutex_lock(&m);
        counter++;
        pthread_mutex_unlock(&m);
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t tid[8];
    int n, i;

    if (argc != 2)
        return 2;
    n = atoi(argv[1]);
    if (n < 1 || n > 8)
        return 2;
    for (i = 0; i < n; i++)
        pthread_create(&tid[i], NULL, add, NULL);
    for (i = 0; i < n; i++)
        pthread_join(tid[i], NULL);
    printf("counter = %ld\n", counter);
    return 0;
}
