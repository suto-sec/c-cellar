#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    const char *dir;
    char cwd[4096];

    if (argc > 2) {
        fprintf(stderr, "usage: mycd [DIR]\n");
        return 2;
    }
    if (argc == 2) {
        dir = argv[1];
    } else if ((dir = getenv("HOME")) == NULL) {
        fprintf(stderr, "mycd: HOME not set\n");
        return 1;
    }
    if (chdir(dir) == -1) {
        fprintf(stderr, "%s: %s\n", dir, strerror(errno));
        return 1;
    }
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        return 1;
    puts(cwd);
    return 0;
}
