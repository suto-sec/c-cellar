#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

static void *square(void *arg)
{
    int n = *(int *)arg;
    int *result = malloc(sizeof *result);

    if (result != NULL)
        *result = n * n;
    pthread_exit(result);
}

int main(int argc, char *argv[])
{
    pthread_t tid;
    void *ret;
    int n;

    if (argc != 2)
        return 2;
    n = atoi(argv[1]);
    pthread_create(&tid, NULL, square, &n);
    pthread_join(tid, &ret);
    if (ret == NULL)
        return 1;
    printf("square: %d\n", *(int *)ret);
    free(ret);
    return 0;
}
