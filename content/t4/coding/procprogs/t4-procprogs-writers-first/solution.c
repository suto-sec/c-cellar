#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

struct job {
    const char *name;
    int writer, start_ms, work_ms;
};

static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t changed = PTHREAD_COND_INITIALIZER;
static int readers, writing, waiting_writers, max_readers, violations;
static char order[64];

static void note(const char *name)
{
    if (order[0] != '\0')
        strcat(order, " ");
    strcat(order, name);
}

static void *run(void *arg)
{
    struct job *j = arg;

    usleep(j->start_ms * 1000);
    pthread_mutex_lock(&lock);
    if (j->writer) {
        waiting_writers++;
        while (writing || readers > 0)
            pthread_cond_wait(&changed, &lock);
        waiting_writers--;
        if (writing || readers > 0)
            violations++;
        writing = 1;
    } else {
        while (writing || waiting_writers > 0)
            pthread_cond_wait(&changed, &lock);
        if (writing)
            violations++;
        readers++;
        if (readers > max_readers)
            max_readers = readers;
    }
    note(j->name);
    pthread_mutex_unlock(&lock);

    usleep(j->work_ms * 1000);

    pthread_mutex_lock(&lock);
    if (j->writer)
        writing = 0;
    else
        readers--;
    pthread_cond_broadcast(&changed);
    pthread_mutex_unlock(&lock);
    return NULL;
}

int main(void)
{
    struct job jobs[4] = {
        {"R0", 0, 0, 300}, {"W1", 1, 100, 100}, {"R2", 0, 150, 50}, {"R3", 0, 220, 50}
    };
    pthread_t th[4];
    int i;

    for (i = 0; i < 4; i++)
        pthread_create(&th[i], NULL, run, &jobs[i]);
    for (i = 0; i < 4; i++)
        pthread_join(th[i], NULL);
    printf("start order: %s\n", order);
    printf("max readers at once: %d\n", max_readers);
    printf("violations: %d\n", violations);
    return 0;
}
