#include <stdio.h>

int main(void)
{
    printf("stdin = %d\n", fileno(stdin));
    printf("stdout = %d\n", fileno(stdout));
    printf("stderr = %d\n", fileno(stderr));
    return 0;
}
