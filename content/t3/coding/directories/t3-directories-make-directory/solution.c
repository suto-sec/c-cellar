#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
        return 2;
    if (mkdir(argv[1], 0755) == -1) {
        fprintf(stderr, "cannot create %s: %s\n", argv[1], strerror(errno));
        return 1;
    }
    printf("created %s\n", argv[1]);
    return 0;
}
