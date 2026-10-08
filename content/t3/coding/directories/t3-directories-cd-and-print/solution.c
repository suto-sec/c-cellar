#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    char cwd[4096];

    if (argc != 2)
        return 2;
    if (chdir(argv[1]) == -1) {
        fprintf(stderr, "%s: %s\n", argv[1], strerror(errno));
        return 1;
    }
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        return 1;
    puts(cwd);
    return 0;
}
