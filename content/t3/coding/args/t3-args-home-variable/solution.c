#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    const char *home = getenv("HOME");

    if (home == NULL)
        printf("HOME is not set\n");
    else
        printf("HOME=%s\n", home);
    return 0;
}
