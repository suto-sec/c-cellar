#include <stdio.h>
#include <string.h>

#define PI 3.14159265

int main(void)
{
    char line[256], cmd[32];
    double a, b;

    while (fgets(line, sizeof(line), stdin) != NULL) {
        if (sscanf(line, "%31s", cmd) != 1)
            continue;
        if (strcmp(cmd, "quit") == 0)
            break;
        if (strcmp(cmd, "circle") == 0 && sscanf(line, "%*s %lf", &a) == 1)
            printf("area=%.2f\n", PI * a * a);
        else if (strcmp(cmd, "rectangle") == 0 && sscanf(line, "%*s %lf %lf", &a, &b) == 2)
            printf("area=%.2f\n", a * b);
        else if (strcmp(cmd, "triangle") == 0 && sscanf(line, "%*s %lf %lf", &a, &b) == 2)
            printf("area=%.2f\n", a * b / 2);
        else
            printf("unknown command\n");
    }
    return 0;
}
