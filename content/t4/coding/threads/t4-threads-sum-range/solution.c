#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

struct job {
    int limit;
    long sum;
};

static void *add_up(void *arg)
{
    struct job *j = arg;
    int i;

    j->sum = 0;
    for (i = 1; i <= j->limit; i++)
        j->sum += i;
    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t tid;
    struct job job;

    if (argc != 2)
        return 2;
    job.limit = atoi(argv[1]);
    job.sum = 0;
    pthread_create(&tid, NULL, add_up, &job);
    pthread_join(tid, NULL);
    printf("sum = %ld\n", job.sum);
    return 0;
}
