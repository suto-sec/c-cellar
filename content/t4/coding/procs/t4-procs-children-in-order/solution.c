#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
    int n, i;

    if (scanf("%d", &n) != 1)
        return 1;
    for (i = 1; i <= n; i++) {
        pid_t pid = fork();

        if (pid < 0)
            return 1;
        if (pid == 0) {
            printf("child %d\n", i);
            exit(0);
        }
        wait(NULL);
    }
    printf("all done\n");
    return 0;
}
