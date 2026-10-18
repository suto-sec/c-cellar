#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct contact {
    char name[32];
    char phone[32];
};

static struct contact *book;
static size_t n, cap;

static long find(const char *name)
{
    for (size_t i = 0; i < n; i++)
        if (strcmp(book[i].name, name) == 0)
            return (long) i;
    return -1;
}

int main(void)
{
    char cmd[32], name[32], phone[32];

    while (scanf("%31s", cmd) == 1 && strcmp(cmd, "quit") != 0) {
        if (strcmp(cmd, "add") == 0) {
            if (scanf("%31s %31s", name, phone) != 2)
                break;
            if (find(name) >= 0) {
                printf("exists\n");
                continue;
            }
            if (n == cap) {
                cap = cap ? cap * 2 : 8;
                book = realloc(book, cap * sizeof(struct contact));
                if (book == NULL)
                    return 1;
            }
            strcpy(book[n].name, name);
            strcpy(book[n].phone, phone);
            n++;
            printf("added\n");
        } else if (strcmp(cmd, "find") == 0) {
            long i;
            if (scanf("%31s", name) != 1)
                break;
            i = find(name);
            if (i < 0)
                printf("not found\n");
            else
                printf("%s: %s\n", book[i].name, book[i].phone);
        } else if (strcmp(cmd, "del") == 0) {
            long i;
            if (scanf("%31s", name) != 1)
                break;
            i = find(name);
            if (i < 0) {
                printf("not found\n");
            } else {
                memmove(&book[i], &book[i + 1], (n - (size_t) i - 1) * sizeof(struct contact));
                n--;
                printf("deleted\n");
            }
        } else if (strcmp(cmd, "list") == 0) {
            if (n == 0)
                printf("(empty)\n");
            for (size_t i = 0; i < n; i++)
                printf("%s %s\n", book[i].name, book[i].phone);
        } else if (strcmp(cmd, "count") == 0) {
            printf("count: %zu\n", n);
        } else {
            printf("unknown command\n");
        }
    }
    free(book);
    return 0;
}
