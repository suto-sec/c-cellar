#include <stdio.h>
#include <pthread.h>

struct sem {
    pthread_mutex_t m;
    pthread_cond_t c;
    int count;
};

static void my_init(struct sem *s, int value)
{
    pthread_mutex_init(&s->m, NULL);
    pthread_cond_init(&s->c, NULL);
    s->count = value;
}

static void my_wait(struct sem *s)
{
    pthread_mutex_lock(&s->m);
    while (s->count == 0)
        pthread_cond_wait(&s->c, &s->m);
    s->count--;
    pthread_mutex_unlock(&s->m);
}

static void my_post(struct sem *s)
{
    pthread_mutex_lock(&s->m);
    s->count++;
    pthread_cond_signal(&s->c);
    pthread_mutex_unlock(&s->m);
}

static struct sem lock;
static long counter = 0;

static void *add(void *arg)
{
    int i;

    (void)arg;
    for (i = 0; i < 100000; i++) {
        my_wait(&lock);
        counter++;
        my_post(&lock);
    }
    return NULL;
}

int main(void)
{
    pthread_t tid[3];
    int i;

    my_init(&lock, 1);
    for (i = 0; i < 3; i++)
        pthread_create(&tid[i], NULL, add, NULL);
    for (i = 0; i < 3; i++)
        pthread_join(tid[i], NULL);
    printf("counter = %ld\n", counter);
    return 0;
}
