#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    int p[2], secs, delay, tick, fd, alive_prod = 1, alive_cons = 1;
    long total = 0;
    ssize_t n;
    char buf[256];
    pid_t prod, cons;

    if (argc != 4)
        return 2;
    secs = atoi(argv[1]);
    delay = atoi(argv[2]);
    if (pipe(p) < 0)
        return 1;
    prod = fork();
    if (prod < 0)
        return 1;
    if (prod == 0) {
        close(p[0]);
        fd = open(argv[3], O_RDONLY);
        if (fd < 0)
            _exit(1);
        while ((n = read(fd, buf, sizeof buf)) > 0)
            write(p[1], buf, n);
        close(fd);
        sleep(delay);
        close(p[1]);
        _exit(0);
    }
    cons = fork();
    if (cons < 0)
        return 1;
    if (cons == 0) {
        close(p[1]);
        while ((n = read(p[0], buf, sizeof buf)) > 0)
            total += n;
        printf("characters: %ld\n", total);
        exit(0);
    }
    close(p[0]);
    close(p[1]);
    for (tick = 0; tick < secs * 10 && (alive_prod || alive_cons); tick++) {
        if (alive_prod && waitpid(prod, NULL, WNOHANG) > 0)
            alive_prod = 0;
        if (alive_cons && waitpid(cons, NULL, WNOHANG) > 0)
            alive_cons = 0;
        if (alive_prod || alive_cons)
            usleep(100000);
    }
    if (!alive_prod && !alive_cons) {
        printf("finished in time\n");
        return 0;
    }
    if (alive_prod) {
        kill(prod, SIGKILL);
        waitpid(prod, NULL, 0);
    }
    if (alive_cons) {
        kill(cons, SIGKILL);
        waitpid(cons, NULL, 0);
    }
    printf("timeout: children aborted\n");
    return 0;
}
