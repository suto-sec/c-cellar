#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int n = 5, count = 0;
    char buf[1024];

    if (argc == 2 && argv[1][0] == '-')
        n = atoi(argv[1] + 1);
    else if (argc != 1) {
        fprintf(stderr, "usage: %s [-N]\n", argv[0]);
        return 1;
    }
    while (count < n && fgets(buf, sizeof(buf), stdin) != NULL) {
        fputs(buf, stdout);
        if (strchr(buf, '\n') != NULL)
            count++;
    }
    return 0;
}
