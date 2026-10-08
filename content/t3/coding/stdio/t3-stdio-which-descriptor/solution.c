#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(void)
{
    const char *text = "hello\n";

    printf("stdout = %d\n", fileno(stdout));
    fflush(stdout);
    FILE *f = fopen("out.txt", "w");
    if (f == NULL)
        return 1;
    printf("file = %d\n", fileno(f));
    fflush(stdout);
    if (write(fileno(f), text, strlen(text)) < 0)
        return 1;
    fclose(f);
    return 0;
}
