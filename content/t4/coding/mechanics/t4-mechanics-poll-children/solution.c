#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
    int delay_ms[4] = {0, 750, 250, 500};
    int i, status, finished = 0, polls = 0;
    pid_t pid;

    for (i = 1; i <= 3; i++) {
        pid = fork();
        if (pid < 0)
            return 1;
        if (pid == 0) {
            usleep(delay_ms[i] * 1000);
            _exit(i);
        }
    }
    while (finished < 3) {
        pid = waitpid(-1, &status, WNOHANG);
        if (pid > 0) {
            printf("child %d finished\n", WEXITSTATUS(status));
            finished++;
        } else {
            polls++;
            usleep(50000);
        }
    }
    printf("%s\n", polls > 5 ? "parent kept working" : "parent was blocked");
    return 0;
}
