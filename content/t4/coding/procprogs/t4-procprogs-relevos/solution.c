#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

static volatile sig_atomic_t go = 0;

static void handler(int sig)
{
    (void)sig;
    go = 1;
}

int main(void)
{
    pid_t next = 0;
    int k;

    signal(SIGUSR1, handler);
    for (k = 4; k >= 1; k--) {
        pid_t pid = fork();

        if (pid < 0)
            return 1;
        if (pid == 0) {
            while (!go)
                pause();
            printf("runner %d running\n", k);
            if (k < 4) {
                printf("runner %d passes the baton to runner %d\n", k, k + 1);
                fflush(stdout);
                kill(next, SIGUSR1);
            } else {
                printf("runner 4 finished the race\n");
            }
            exit(0);
        }
        next = pid;
    }
    printf("all runners created\n");
    fflush(stdout);
    sleep(1);
    printf("go!\n");
    fflush(stdout);
    kill(next, SIGUSR1);
    while (wait(NULL) > 0)
        ;
    printf("all runners finished\n");
    return 0;
}
