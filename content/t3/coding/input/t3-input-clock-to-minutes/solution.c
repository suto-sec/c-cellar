#include <stdio.h>

int main(void)
{
    char line[64];
    int h, m;

    if (fgets(line, sizeof(line), stdin) == NULL)
        return 1;
    if (sscanf(line, "%d:%d", &h, &m) != 2)
        return 1;
    printf("%d\n", h * 60 + m);
    return 0;
}
