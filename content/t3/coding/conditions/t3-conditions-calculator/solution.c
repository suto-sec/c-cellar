#include <stdio.h>

int main(void)
{
    int a, b;
    char op;

    if (scanf("%d %c %d", &a, &op, &b) != 3)
        return 1;
    switch (op) {
        case '+': printf("%d\n", a + b); break;
        case '-': printf("%d\n", a - b); break;
        case '*': printf("%d\n", a * b); break;
        case '/':
            if (b == 0)
                printf("error\n");
            else
                printf("%d\n", a / b);
            break;
        default:
            printf("unknown operator\n");
    }
    return 0;
}
