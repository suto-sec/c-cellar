#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

static int list[8000];
static int count = 0;
static pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

static void *append(void *arg)
{
    int value = *(int *)arg;
    int k;

    for (k = 0; k < 1000; k++) {
        pthread_mutex_lock(&m);
        list[count] = value;
        count++;
        pthread_mutex_unlock(&m);
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t tid[8];
    int vals[8], n, i;
    long sum = 0;

    if (argc != 2)
        return 2;
    n = atoi(argv[1]);
    if (n < 1 || n > 8)
        return 2;
    for (i = 0; i < n; i++) {
        vals[i] = i + 1;
        pthread_create(&tid[i], NULL, append, &vals[i]);
    }
    for (i = 0; i < n; i++)
        pthread_join(tid[i], NULL);
    for (i = 0; i < count; i++)
        sum += list[i];
    printf("entries: %d\nsum: %ld\n", count, sum);
    return 0;
}
