#include <stdio.h>
#include <stdlib.h>

struct node {
    int value;
    struct node *next;
};

int main(void)
{
    struct node *head = NULL, *n;
    int x;

    while (scanf("%d", &x) == 1) {
        n = malloc(sizeof(struct node));
        if (n == NULL)
            return 1;
        n->value = x;
        n->next = head;
        head = n;
    }
    for (n = head; n != NULL; n = n->next)
        printf("%d%s", n->value, n->next ? " " : "");
    printf("\n");
    while (head != NULL) {
        n = head->next;
        free(head);
        head = n;
    }
    return 0;
}
