#include <stdio.h>
#include <string.h>
#include <pthread.h>

#define SIZE 10000
#define PASSES 200

static int buffer[SIZE];
static int use_mutex = 0;
static int bad = 0;
static pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

static void *writer(void *arg)
{
    int p, i;

    (void)arg;
    for (p = 1; p <= PASSES; p++) {
        if (use_mutex)
            pthread_mutex_lock(&m);
        for (i = 0; i < SIZE; i++)
            buffer[i] = p;
        if (use_mutex)
            pthread_mutex_unlock(&m);
    }
    return NULL;
}

static void *reader(void *arg)
{
    int p, i;

    (void)arg;
    for (p = 0; p < PASSES; p++) {
        int ok = 1;

        if (use_mutex)
            pthread_mutex_lock(&m);
        for (i = 1; i < SIZE; i++)
            if (buffer[i] != buffer[0])
                ok = 0;
        if (use_mutex)
            pthread_mutex_unlock(&m);
        if (!ok)
            bad++;
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t w, r;

    use_mutex = argc > 1 && strcmp(argv[1], "mutex") == 0;
    pthread_create(&w, NULL, writer, NULL);
    pthread_create(&r, NULL, reader, NULL);
    pthread_join(w, NULL);
    pthread_join(r, NULL);
    if (bad == 0)
        printf("all reads consistent\n");
    else
        printf("inconsistent reads: %d\n", bad);
    return 0;
}
