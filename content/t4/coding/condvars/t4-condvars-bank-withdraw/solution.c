#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

static pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t enough = PTHREAD_COND_INITIALIZER;
static int balance = 0, withdrawn = 0;

static void *bank(void *arg)
{
    int i;

    (void)arg;
    for (i = 0; i < 2; i++) {
        sleep(1);
        pthread_mutex_lock(&m);
        balance += 3000;
        pthread_cond_signal(&enough);
        pthread_mutex_unlock(&m);
    }
    return NULL;
}

static void *employee(void *arg)
{
    int i;

    (void)arg;
    for (i = 0; i < 6; i++) {
        pthread_mutex_lock(&m);
        while (balance < 1000)
            pthread_cond_wait(&enough, &m);
        balance -= 1000;
        withdrawn += 1000;
        pthread_mutex_unlock(&m);
    }
    return NULL;
}

int main(void)
{
    pthread_t b, e;

    pthread_create(&b, NULL, bank, NULL);
    pthread_create(&e, NULL, employee, NULL);
    pthread_join(b, NULL);
    pthread_join(e, NULL);
    printf("withdrew %d\nbalance %d\n", withdrawn, balance);
    return 0;
}
