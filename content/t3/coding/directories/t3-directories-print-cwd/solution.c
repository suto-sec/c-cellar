#include <stdio.h>
#include <unistd.h>

int main(void)
{
    char cwd[4096];

    if (getcwd(cwd, sizeof(cwd)) == NULL)
        return 1;
    puts(cwd);
    return 0;
}
