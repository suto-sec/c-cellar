#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

static pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cv = PTHREAD_COND_INITIALIZER;
static int turn = 0;
static int rounds;

static void *letter(void *arg)
{
    int k = *(int *)arg;
    int i;

    for (i = 0; i < rounds; i++) {
        pthread_mutex_lock(&m);
        while (turn != k)
            pthread_cond_wait(&cv, &m);
        putchar('A' + k);
        turn = (turn + 1) % 3;
        pthread_cond_broadcast(&cv);
        pthread_mutex_unlock(&m);
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t tid[3];
    int ids[3], i;

    if (argc != 2)
        return 2;
    rounds = atoi(argv[1]);
    for (i = 0; i < 3; i++) {
        ids[i] = i;
        pthread_create(&tid[i], NULL, letter, &ids[i]);
    }
    for (i = 0; i < 3; i++)
        pthread_join(tid[i], NULL);
    putchar('\n');
    return 0;
}
