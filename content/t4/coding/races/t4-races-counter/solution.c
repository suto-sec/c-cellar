#include <stdio.h>
#include <pthread.h>

static long counter = 0;
static pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

static void *add(void *arg)
{
    int i;

    (void)arg;
    for (i = 0; i < 500000; i++) {
        pthread_mutex_lock(&m);
        counter++;
        pthread_mutex_unlock(&m);
    }
    return NULL;
}

int main(void)
{
    pthread_t a, b;

    pthread_create(&a, NULL, add, NULL);
    pthread_create(&b, NULL, add, NULL);
    pthread_join(a, NULL);
    pthread_join(b, NULL);
    printf("counter = %ld\n", counter);
    return 0;
}
