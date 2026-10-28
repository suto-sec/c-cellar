#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/wait.h>

static int x = 1;

static void *worker(void *arg)
{
    (void)arg;
    x = 2;
    return NULL;
}

int main(void)
{
    pthread_t tid;
    pid_t pid;

    pthread_create(&tid, NULL, worker, NULL);
    pthread_join(tid, NULL);
    printf("after thread: x = %d\n", x);
    fflush(stdout);
    pid = fork();
    if (pid < 0)
        return 1;
    if (pid == 0) {
        x = 3;
        exit(0);
    }
    wait(NULL);
    printf("after child process: x = %d\n", x);
    return 0;
}
