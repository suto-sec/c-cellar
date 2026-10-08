#include <dirent.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    struct dirent *e;
    int count = 0;

    if (argc != 2)
        return 1;
    DIR *d = opendir(argv[1]);
    if (d == NULL)
        return 1;
    while ((e = readdir(d)) != NULL)
        if (strcmp(e->d_name, ".") != 0 && strcmp(e->d_name, "..") != 0)
            count++;
    printf("%d entries\n", count);
    rewinddir(d);
    while ((e = readdir(d)) != NULL)
        if (strcmp(e->d_name, ".") != 0 && strcmp(e->d_name, "..") != 0)
            puts(e->d_name);
    closedir(d);
    return 0;
}
