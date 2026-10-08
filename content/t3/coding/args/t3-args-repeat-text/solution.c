#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int n;

    if (argc != 3 || (n = atoi(argv[1])) < 0) {
        fprintf(stderr, "usage: repeat N TEXT\n");
        return 2;
    }
    for (int i = 0; i < n; i++)
        puts(argv[2]);
    return 0;
}
