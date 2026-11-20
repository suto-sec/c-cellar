#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

static void redirect(const char *file, int flags, int target)
{
    int fd = open(file, flags, 0644);

    if (fd < 0) {
        fprintf(stderr, "%s: Error. %s\n", file, strerror(errno));
        _exit(1);
    }
    dup2(fd, target);
    close(fd);
}

int main(int argc, char *argv[])
{
    int status;
    pid_t pid;

    if (argc < 4)
        return 2;
    pid = fork();
    if (pid < 0)
        return 1;
    if (pid == 0) {
        redirect(argv[1], O_RDONLY, STDIN_FILENO);
        redirect(argv[2], O_WRONLY | O_CREAT | O_TRUNC, STDOUT_FILENO);
        redirect(argv[3], O_WRONLY | O_CREAT | O_TRUNC, STDERR_FILENO);
        execlp("sh", "sh", "-c", "sort; echo sort done >&2", (char *)NULL);
        _exit(127);
    }
    waitpid(pid, &status, 0);
    printf("exit status %d\n", WEXITSTATUS(status));
    return 0;
}
