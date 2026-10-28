#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SLOTS 5

static int buffer[SLOTS];
static int in = 0, out = 0, count = 0, max_count = 0;
static int total;
static long sum = 0;
static pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t has_room = PTHREAD_COND_INITIALIZER;
static pthread_cond_t has_data = PTHREAD_COND_INITIALIZER;

static void *producer(void *arg)
{
    int i;

    (void)arg;
    for (i = 1; i <= total; i++) {
        pthread_mutex_lock(&m);
        while (count == SLOTS)
            pthread_cond_wait(&has_room, &m);
        buffer[in] = i;
        in = (in + 1) % SLOTS;
        count++;
        if (count > max_count)
            max_count = count;
        pthread_cond_signal(&has_data);
        pthread_mutex_unlock(&m);
    }
    return NULL;
}

static void *consumer(void *arg)
{
    int i, v;

    (void)arg;
    for (i = 0; i < total; i++) {
        pthread_mutex_lock(&m);
        while (count == 0)
            pthread_cond_wait(&has_data, &m);
        v = buffer[out];
        out = (out + 1) % SLOTS;
        count--;
        pthread_cond_signal(&has_room);
        pthread_mutex_unlock(&m);
        sum += v;
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t p, c;

    if (argc != 2)
        return 2;
    total = atoi(argv[1]);
    pthread_create(&p, NULL, producer, NULL);
    pthread_create(&c, NULL, consumer, NULL);
    pthread_join(p, NULL);
    pthread_join(c, NULL);
    printf("sum = %ld\noverflow: %s\n", sum, max_count > SLOTS ? "yes" : "no");
    return 0;
}
