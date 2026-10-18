#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int first = 1, inc = 1, last;

    if (argc == 2) {
        last = atoi(argv[1]);
    } else if (argc == 3) {
        first = atoi(argv[1]);
        last = atoi(argv[2]);
    } else if (argc == 4) {
        first = atoi(argv[1]);
        inc = atoi(argv[2]);
        last = atoi(argv[3]);
    } else {
        fprintf(stderr, "usage: myseq [FIRST [INCREMENT]] LAST\n");
        return 2;
    }
    if (inc == 0) {
        fprintf(stderr, "myseq: the increment cannot be 0\n");
        return 1;
    }
    if (inc > 0)
        for (int i = first; i <= last; i += inc)
            printf("%d\n", i);
    else
        for (int i = first; i >= last; i += inc)
            printf("%d\n", i);
    return 0;
}
