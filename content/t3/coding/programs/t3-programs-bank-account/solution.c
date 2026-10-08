#include <stdio.h>
#include <string.h>

struct account {
    long balance;
};

static void deposit(struct account *a, long n)
{
    a->balance += n;
    printf("ok\n");
}

static void withdraw(struct account *a, long n)
{
    if (n > a->balance)
        printf("insufficient funds\n");
    else {
        a->balance -= n;
        printf("ok\n");
    }
}

int main(void)
{
    struct account acc = {0};
    char line[128], cmd[32];
    long n;

    while (fgets(line, sizeof(line), stdin) != NULL) {
        if (sscanf(line, "%31s", cmd) != 1)
            continue;
        if (strcmp(cmd, "deposit") == 0 && sscanf(line, "%*s %ld", &n) == 1)
            deposit(&acc, n);
        else if (strcmp(cmd, "withdraw") == 0 && sscanf(line, "%*s %ld", &n) == 1)
            withdraw(&acc, n);
        else if (strcmp(cmd, "balance") == 0)
            printf("balance: %ld\n", acc.balance);
        else
            printf("unknown command\n");
    }
    return 0;
}
