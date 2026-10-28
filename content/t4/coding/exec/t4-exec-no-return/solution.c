#include <stdio.h>
#include <unistd.h>

int main(void)
{
    printf("before\n");
    fflush(stdout);
    execlp("echo", "echo", "replaced", NULL);
    printf("after\n");
    return 1;
}
