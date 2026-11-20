#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t changed = PTHREAD_COND_INITIALIZER;
static int t1, t2, max1, max2, made, goal, done, used1, used2;

static void *maker(void *arg)
{
    int *count = arg;
    int *max = (count == &t1) ? &max1 : &max2;

    for (;;) {
        pthread_mutex_lock(&lock);
        while (*count == 2 && !done)
            pthread_cond_wait(&changed, &lock);
        if (done) {
            pthread_mutex_unlock(&lock);
            return NULL;
        }
        (*count)++;
        if (*count > *max)
            *max = *count;
        pthread_cond_broadcast(&changed);
        pthread_mutex_unlock(&lock);
    }
}

static void *assembler(void *arg)
{
    (void)arg;
    while (made < goal) {
        pthread_mutex_lock(&lock);
        while (t1 < 2 || t2 < 2)
            pthread_cond_wait(&changed, &lock);
        t1 -= 2;
        t2 -= 2;
        used1 += 2;
        used2 += 2;
        made++;
        if (made == goal)
            done = 1;
        pthread_cond_broadcast(&changed);
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t r1, r2, r3;

    if (argc != 2 || (goal = atoi(argv[1])) < 1)
        return 2;
    pthread_create(&r1, NULL, maker, &t1);
    pthread_create(&r2, NULL, maker, &t2);
    pthread_create(&r3, NULL, assembler, NULL);
    pthread_join(r3, NULL);
    pthread_join(r1, NULL);
    pthread_join(r2, NULL);
    printf("T3 made: %d\n", made);
    printf("tray limits respected: %s\n", max1 <= 2 && max2 <= 2 ? "yes" : "no");
    printf("pieces used: T1=%d T2=%d\n", used1, used2);
    return 0;
}
