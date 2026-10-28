#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

static int out_fd;

static void handler(int sig)
{
    char buf[4];
    int n = rand() % 11, len = 0;

    (void)sig;
    if (n == 10)
        buf[len++] = '1';
    buf[len++] = (n == 10) ? '0' : (char)('0' + n);
    buf[len++] = '\n';
    write(out_fd, buf, len);
}

int main(void)
{
    int p[2];
    pid_t pid;
    char line[256], buf[16];
    ssize_t k;

    if (pipe(p) < 0)
        return 1;
    out_fd = p[1];
    signal(SIGUSR1, handler);
    pid = fork();
    if (pid < 0)
        return 1;
    if (pid == 0) {
        close(p[0]);
        srand(getpid());
        for (;;)
            pause();
    }
    close(p[1]);
    while (fgets(line, sizeof line, stdin) != NULL) {
        kill(pid, SIGUSR1);
        k = read(p[0], buf, sizeof buf - 1);
        if (k <= 0)
            break;
        buf[k] = '\0';
        fputs(buf, stdout);
    }
    kill(pid, SIGTERM);
    wait(NULL);
    return 0;
}
