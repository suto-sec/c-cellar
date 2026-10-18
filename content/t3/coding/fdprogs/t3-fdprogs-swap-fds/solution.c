#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int tmp;
    const char one[] = "to descriptor 1\n", two[] = "to descriptor 2\n";

    (void) argv;
    if (argc != 1) {
        fprintf(stderr, "usage: swapfds\n");
        return 2;
    }
    tmp = dup(1);
    dup2(2, 1);
    dup2(tmp, 2);
    close(tmp);
    if (write(1, one, strlen(one)) == -1 || write(2, two, strlen(two)) == -1)
        return 1;
    return 0;
}
