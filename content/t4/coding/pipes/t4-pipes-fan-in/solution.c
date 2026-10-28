#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    int p[2], n, i;
    char buf[256];
    ssize_t k;

    if (argc != 2)
        return 2;
    n = atoi(argv[1]);
    if (pipe(p) < 0)
        return 1;
    for (i = 1; i <= n; i++) {
        pid_t pid = fork();

        if (pid < 0)
            return 1;
        if (pid == 0) {
            char line[] = "worker 0\n";

            close(p[0]);
            line[7] = '0' + i;
            write(p[1], line, strlen(line));
            close(p[1]);
            exit(0);
        }
    }
    close(p[1]);
    while ((k = read(p[0], buf, sizeof buf)) > 0)
        write(STDOUT_FILENO, buf, k);
    close(p[0]);
    while (wait(NULL) > 0)
        ;
    return 0;
}
