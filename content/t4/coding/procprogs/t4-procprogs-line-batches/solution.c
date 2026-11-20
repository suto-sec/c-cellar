#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

static volatile sig_atomic_t go;

static void on_usr1(int sig)
{
    (void)sig;
    go = 1;
}

int main(int argc, char *argv[])
{
    int fd, k, child, total = 0, status, lines;
    pid_t pid;
    char c;

    if (argc != 3)
        return 2;
    k = atoi(argv[2]);
    if (k < 1 || k > 255)
        return 2;
    fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        perror(argv[1]);
        return 1;
    }
    signal(SIGUSR1, on_usr1);
    for (child = 1; ; child++) {
        pid = fork();
        if (pid < 0)
            return 1;
        if (pid == 0) {
            while (!go)
                pause();
            lines = 0;
            while (lines < k && read(fd, &c, 1) == 1)
                if (c == '\n')
                    lines++;
            printf("Process %d: read %d lines\n", child, lines);
            exit(lines);
        }
        usleep(100000);
        kill(pid, SIGUSR1);
        waitpid(pid, &status, 0);
        lines = WEXITSTATUS(status);
        total += lines;
        if (lines < k)
            break;
    }
    printf("Parent: the file has %d lines\n", total);
    return 0;
}
