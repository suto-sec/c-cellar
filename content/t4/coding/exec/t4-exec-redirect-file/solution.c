#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    pid_t pid;

    if (argc < 3)
        return 2;
    pid = fork();
    if (pid < 0)
        return 1;
    if (pid == 0) {
        int fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0644);

        if (fd < 0)
            exit(1);
        dup2(fd, STDOUT_FILENO);
        close(fd);
        execvp(argv[2], &argv[2]);
        exit(127);
    }
    wait(NULL);
    printf("saved\n");
    return 0;
}
