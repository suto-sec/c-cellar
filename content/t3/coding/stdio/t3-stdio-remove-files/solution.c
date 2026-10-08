#include <errno.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int status = 0;

    for (int i = 1; i < argc; i++) {
        if (remove(argv[i]) != 0) {
            fprintf(stderr, "remove %s: %s\n", argv[i], strerror(errno));
            status = 1;
        }
    }
    return status;
}
