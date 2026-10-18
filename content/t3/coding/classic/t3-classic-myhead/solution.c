#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    char buf[1024];
    int n = 10, count = 0;

    if (argc == 2 && argv[1][0] == '-')
        n = atoi(argv[1] + 1);
    else if (argc != 1) {
        fprintf(stderr, "usage: myhead [-N]\n");
        return 2;
    }
    while (count < n && fgets(buf, 1024, stdin) != NULL) {
        fputs(buf, stdout);
        count++;
    }
    return 0;
}
