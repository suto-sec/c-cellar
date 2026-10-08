#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int first, last, step;

    if (argc != 3 && argc != 4) {
        fprintf(stderr, "usage: countrange FIRST LAST [STEP]\n");
        return 2;
    }
    first = atoi(argv[1]);
    last = atoi(argv[2]);
    step = argc == 4 ? atoi(argv[3]) : (first <= last ? 1 : -1);
    if (step == 0 || (step > 0 && first > last) || (step < 0 && first < last)) {
        fprintf(stderr, "bad step\n");
        return 1;
    }
    for (int x = first; step > 0 ? x <= last : x >= last; x += step)
        printf("%s%d", x == first ? "" : " ", x);
    printf("\n");
    return 0;
}
