#include <stdio.h>

#define MAXP 10
#define MAXB 9
#define MAXT 2000
#define FOREVER 1000000

struct proc {
    int arr, k, burst[MAXB];
    int idx, rem, seq;
    int finish, ready_ms, disk_ms;
    char state;   /* '.' not arrived, 'L' ready, 'E' running, 'W' waiting for the disk, 'D' using the disk, 'T' terminated */
};

static struct proc p[MAXP];
static char tl[MAXP][MAXT];
static int n, next_seq = 1;

static void make_ready(int i)
{
    p[i].state = 'L';
    p[i].seq = next_seq++;
}

/* the ready process that has waited longest */
static int pick(void)
{
    int i, best = -1;

    for (i = 0; i < n; i++)
        if (p[i].state == 'L' && (best < 0 || p[i].seq < p[best].seq))
            best = i;
    return best;
}

int main(void)
{
    int i, j, t, cpu = -1, disk = -1, done = 0, dq[4 * MAXP], dq_head = 0, dq_len = 0;
    long total = 0;

    if (scanf("%d", &n) != 1 || n < 1 || n > MAXP)
        return 1;
    for (i = 0; i < n; i++) {
        if (scanf("%d", &p[i].arr) != 1)
            return 1;
        if (scanf("%d", &p[i].k) != 1 || p[i].k < 1 || p[i].k > MAXB)
            return 1;
        for (j = 0; j < p[i].k; j++)
            if (scanf("%d", &p[i].burst[j]) != 1)
                return 1;
        p[i].rem = p[i].burst[0];
        p[i].finish = -1;
        p[i].state = '.';
    }

    for (t = 0; ; t++) {
        /* 1. a disk burst ends: the process is ready again */
        if (disk >= 0 && p[disk].rem == 0) {
            p[disk].idx++;
            p[disk].rem = p[disk].burst[p[disk].idx];
            make_ready(disk);
            disk = -1;
        }
        /* 2. a CPU burst ends: ask for the disk, or terminate */
        if (cpu >= 0 && p[cpu].rem == 0) {
            p[cpu].idx++;
            if (p[cpu].idx == p[cpu].k) {
                p[cpu].finish = t;
                p[cpu].state = 'T';
                done++;
            } else {
                p[cpu].rem = p[cpu].burst[p[cpu].idx];
                p[cpu].state = 'W';
                dq[dq_len++] = cpu;
            }
            cpu = -1;
        }
        if (done == n)
            break;
        /* 3. arrivals */
        for (i = 0; i < n; i++)
            if (p[i].arr == t)
                make_ready(i);
        /* 4. the free disk takes the first request */
        if (disk < 0 && dq_head < dq_len) {
            disk = dq[dq_head++];
            p[disk].state = 'D';
        }
        /* 5. the CPU */
        if (cpu < 0)
            cpu = pick();
        if (cpu >= 0)
            p[cpu].state = 'E';
        /* one millisecond goes by */
        for (i = 0; i < n; i++) {
            char c = p[i].state;

            if (c == 'L')
                p[i].ready_ms++;
            if (c == 'W')
                p[i].disk_ms++;
            tl[i][t] = c;
        }
        if (cpu >= 0) {
            p[cpu].rem--;
        }
        if (disk >= 0)
            p[disk].rem--;
    }

    for (i = 0; i < n; i++) {
        printf("P%d: finish %d ready %d disk-wait %d\n", i + 1, p[i].finish, p[i].ready_ms, p[i].disk_ms);
        total += p[i].finish - p[i].arr;
    }
    printf("average turnaround: %.2f\n", (double)total / n);
    return 0;
}
