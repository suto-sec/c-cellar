#include <stdio.h>
#include <signal.h>

int main(void)
{
    signal(SIGINT, SIG_IGN);
    raise(SIGINT);
    printf("still alive\n");
    return 0;
}
