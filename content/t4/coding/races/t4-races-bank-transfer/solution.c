#include <stdio.h>
#include <pthread.h>

static long a = 1000, b = 1000;
static pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

static void *a_to_b(void *arg)
{
    int i;

    (void)arg;
    for (i = 0; i < 10000; i++) {
        pthread_mutex_lock(&m);
        a--;
        b++;
        pthread_mutex_unlock(&m);
    }
    return NULL;
}

static void *b_to_a(void *arg)
{
    int i;

    (void)arg;
    for (i = 0; i < 10000; i++) {
        pthread_mutex_lock(&m);
        b--;
        a++;
        pthread_mutex_unlock(&m);
    }
    return NULL;
}

int main(void)
{
    pthread_t t1, t2;

    pthread_create(&t1, NULL, a_to_b, NULL);
    pthread_create(&t2, NULL, b_to_a, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    printf("a=%ld b=%ld total=%ld\n", a, b, a + b);
    return 0;
}
