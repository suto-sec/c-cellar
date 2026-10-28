#include <stdio.h>
#include <semaphore.h>

int main(void)
{
    sem_t s;
    int v;

    sem_init(&s, 0, 2);
    sem_getvalue(&s, &v);
    printf("initial: %d\n", v);
    sem_wait(&s);
    sem_getvalue(&s, &v);
    printf("after wait: %d\n", v);
    sem_wait(&s);
    sem_getvalue(&s, &v);
    printf("after second wait: %d\n", v);
    sem_post(&s);
    sem_getvalue(&s, &v);
    printf("after post: %d\n", v);
    sem_destroy(&s);
    return 0;
}
