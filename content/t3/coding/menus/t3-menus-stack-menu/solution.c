#include <stdio.h>

#define CAP 5

static int stack[CAP];
static int count;

static int push(int v)
{
    if (count == CAP)
        return -1;
    stack[count++] = v;
    return 0;
}

static int pop(int *v)
{
    if (count == 0)
        return -1;
    *v = stack[--count];
    return 0;
}

int main(void)
{
    int option, v;

    while (scanf("%d", &option) == 1 && option != 0) {
        switch (option) {
        case 1:
            if (scanf("%d", &v) != 1)
                return 0;
            if (push(v) == -1)
                printf("stack full\n");
            break;
        case 2:
            if (pop(&v) == -1)
                printf("stack empty\n");
            else
                printf("popped %d\n", v);
            break;
        case 3:
            if (count == 0)
                printf("stack empty\n");
            else
                printf("top %d\n", stack[count - 1]);
            break;
        case 4:
            printf("size %d\n", count);
            break;
        default:
            printf("invalid option\n");
        }
    }
    return 0;
}
