#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    char line[256];
    int count = 0;

    if (argc != 2)
        return 1;
    int fd = open(argv[1], O_RDONLY);
    if (fd < 0)
        return 1;
    FILE *f = fdopen(fd, "r");
    if (f == NULL) {
        close(fd);
        return 1;
    }
    while (fgets(line, sizeof line, f) != NULL)
        count++;
    fclose(f);
    printf("lines = %d\n", count);
    return 0;
}
