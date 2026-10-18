#include <stdio.h>

int main(void)
{
    if (rename("old.txt", "new.txt") == -1) {
        printf("cannot rename\n");
        return 1;
    }
    printf("renamed\n");
    return 0;
}
