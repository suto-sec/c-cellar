#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
    int p[2];
    pid_t pid;
    char buf[512];
    ssize_t n;
    long total = 0;

    if (pipe(p) < 0)
        return 1;
    pid = fork();
    if (pid < 0)
        return 1;
    if (pid == 0) {
        close(p[1]);
        while ((n = read(p[0], buf, sizeof buf)) > 0)
            total += n;
        close(p[0]);
        printf("bytes: %ld\n", total);
        exit(0);
    }
    close(p[0]);
    while ((n = read(STDIN_FILENO, buf, sizeof buf)) > 0)
        write(p[1], buf, n);
    close(p[1]);
    wait(NULL);
    return 0;
}
