#include <stdio.h>

int main(void)
{
    if (remove("junk.txt") == -1) {
        printf("nothing to remove\n");
        return 1;
    }
    printf("removed\n");
    return 0;
}
