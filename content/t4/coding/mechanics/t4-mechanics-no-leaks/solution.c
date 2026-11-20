#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
    int round, total = 0, n, fd;
    char buf[64];

    for (round = 0; round < 200; round++) {
        int p[2];
        pid_t pid;

        if (pipe(p) < 0) {
            perror("pipe");
            return 1;
        }
        pid = fork();
        if (pid < 0)
            return 1;
        if (pid == 0) {
            dup2(p[1], STDOUT_FILENO);
            close(p[0]);
            close(p[1]);
            execlp("echo", "echo", "hi", (char *)NULL);
            _exit(127);
        }
        close(p[1]);
        while ((n = read(p[0], buf, sizeof buf)) > 0)
            total += n;
        close(p[0]);
        waitpid(pid, NULL, 0);
    }
    fd = dup(0);
    printf("bytes: %d\nnext free descriptor: %d\n", total, fd);
    return 0;
}
