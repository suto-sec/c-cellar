#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

struct store {
    int *item;
    int slots, in, out, count, max;
    sem_t room, filled;
    pthread_mutex_t lock;
};

static struct store chassis, engines;
static int cars, made;

static void store_init(struct store *s, int slots)
{
    s->item = malloc(slots * sizeof *s->item);
    s->slots = slots;
    s->in = s->out = s->count = s->max = 0;
    sem_init(&s->room, 0, slots);
    sem_init(&s->filled, 0, 0);
    pthread_mutex_init(&s->lock, NULL);
}

static void store_free(struct store *s)
{
    free(s->item);
    sem_destroy(&s->room);
    sem_destroy(&s->filled);
    pthread_mutex_destroy(&s->lock);
}

static void put(struct store *s, int v)
{
    sem_wait(&s->room);
    pthread_mutex_lock(&s->lock);
    s->item[s->in] = v;
    s->in = (s->in + 1) % s->slots;
    s->count++;
    if (s->count > s->max)
        s->max = s->count;
    pthread_mutex_unlock(&s->lock);
    sem_post(&s->filled);
}

static int get(struct store *s)
{
    int v;

    sem_wait(&s->filled);
    pthread_mutex_lock(&s->lock);
    v = s->item[s->out];
    s->out = (s->out + 1) % s->slots;
    s->count--;
    pthread_mutex_unlock(&s->lock);
    sem_post(&s->room);
    return v;
}

static void *maker(void *arg)
{
    struct store *s = arg;
    int i;

    for (i = 0; i < cars; i++)
        put(s, i);
    return NULL;
}

static void *assembler(void *arg)
{
    int i;

    (void)arg;
    for (i = 0; i < cars; i++) {
        get(&chassis);
        get(&engines);
        made++;
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t a, b, c;
    int ok;

    if (argc != 4)
        return 2;
    cars = atoi(argv[3]);
    if (atoi(argv[1]) < 1 || atoi(argv[2]) < 1 || cars < 1)
        return 2;
    store_init(&chassis, atoi(argv[1]));
    store_init(&engines, atoi(argv[2]));
    pthread_create(&a, NULL, maker, &chassis);
    pthread_create(&b, NULL, maker, &engines);
    pthread_create(&c, NULL, assembler, NULL);
    pthread_join(a, NULL);
    pthread_join(b, NULL);
    pthread_join(c, NULL);
    ok = chassis.max <= chassis.slots && engines.max <= engines.slots;
    printf("cars: %d\n", made);
    printf("store limits respected: %s\n", ok ? "yes" : "no");
    printf("left over: %d chassis, %d engines\n", chassis.count, engines.count);
    store_free(&chassis);
    store_free(&engines);
    printf("Factory closed\n");
    return 0;
}
