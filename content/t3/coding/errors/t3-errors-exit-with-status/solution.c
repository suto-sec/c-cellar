#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc < 2)
        exit(0);
    exit(atoi(argv[1]));
}
