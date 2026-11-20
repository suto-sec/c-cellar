"""Daily exercise templates of T4 (see app/daily.py). Each template gets a random.Random and returns a spec:
title, stars, summary, statement, hints, tests (+ solution).
Expected outputs are computed here in Python from the statement, never by running the solution. Only the C of the T3/T4 syllabus is used."""
from pathlib import Path
from string import Template

HERE = Path(__file__).resolve().parent


def C(src, /, **kw):
    """C source from a $name template (C has no $, so this is safe with braces)."""
    return Template(src).substitute(**kw)


# ------------------------------------------------------------------ procs


def procs_children(r):
    a, b = r.randint(2, 9), r.randint(1, 9)
    reverse = r.random() < 0.5
    status = lambda i: (a * i + b) % 200 + 1
    if reverse:
        how = "First create **all** the children, then collect them with `waitpid` in **reverse** order of creation (the last child first)."
        body = '''    pid_t pid[16];
    int i, st;

    for (i = 1; i <= n; i++) {
        pid[i] = fork();
        if (pid[i] < 0)
            return 1;
        if (pid[i] == 0)
            exit((A * i + B) % 200 + 1);
    }
    for (i = n; i >= 1; i--) {
        waitpid(pid[i], &st, 0);
        printf("child %d: %d\\n", i, WEXITSTATUS(st));
    }
'''
        hints = ["Keep the pids in an array so that you can wait for a particular one with `waitpid(pid, &status, 0)`.",
                 "Create every child inside the first loop and collect them in a second loop that counts down."]
    else:
        how = "Create the children **one at a time**: create child `i`, wait for it with `waitpid`, print its line, and only then create the next."
        body = '''    pid_t pid;
    int i, st;

    for (i = 1; i <= n; i++) {
        pid = fork();
        if (pid < 0)
            return 1;
        if (pid == 0)
            exit((A * i + B) % 200 + 1);
        waitpid(pid, &st, 0);
        printf("child %d: %d\\n", i, WEXITSTATUS(st));
        fflush(stdout);
    }
'''
        hints = ["Inside the loop: `fork`, the child calls `exit(...)`, the parent calls `waitpid` and prints.",
                 "`WEXITSTATUS(status)` gives the number passed to `exit`.",
                 "If lines appear more than once when the output goes to a pipe or a file, the parent had text waiting in its buffer when it forked: `fflush(stdout)` before every `fork`."]
    tests = []
    for n in (1, 3, r.randint(4, 8), 12):
        order = range(n, 0, -1) if reverse else range(1, n + 1)
        tests.append({"args": [str(n)], "stdout": "".join(f"child {i}: {status(i)}\n" for i in order)})
    statement = (f"`prog N` (with `N` from 1 to 15) creates `N` children, numbered from 1. Child number `i` ends with the exit status `({a} * i + {b}) % 200 + 1`.\n\n{how} "
                 "Each line printed by the parent is `child I: S`, with `S` the exit status of child `I`.")
    solution = C('''#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define A $a
#define B $b

int main(int argc, char *argv[])
{
    int n;

    if (argc != 2)
        return 2;
    n = atoi(argv[1]);
    if (n < 1 || n > 15)
        return 2;
$body    return 0;
}
''', a=a, b=b, body=body)
    return {"title": "Collect the children" + (" backwards" if reverse else " one by one"), "stars": 2 if not reverse else 3,
            "summary": "Children that end with a computed status, collected in a given order.", "statement": statement, "hints": hints, "tests": tests, "solution": solution}


# ------------------------------------------------------------------ exec


def exec_wrapper(r):
    if r.random() < 0.5:
        sep = r.choice([",", ";", "-", ":"])
        tests = []
        for _ in range(4):
            lo = r.randint(1, 20)
            hi = lo + r.randint(1, 8)
            tests.append({"args": [str(lo), str(hi)], "stdout": sep.join(str(i) for i in range(lo, hi + 1)) + "\nexit status 0\n"})
        statement = (f"`prog FIRST LAST` runs the command `seq -s {sep} FIRST LAST` in a child (`seq` prints the numbers from `FIRST` to `LAST` with the separator `{sep}` between them). "
                     "The parent waits and then prints `exit status S`, where `S` is the status of the child.")
        solution = C('''#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    int st;
    pid_t pid;

    if (argc != 3)
        return 2;
    pid = fork();
    if (pid < 0)
        return 1;
    if (pid == 0) {
        execlp("seq", "seq", "-s", "$sep", argv[1], argv[2], (char *)NULL);
        _exit(127);
    }
    waitpid(pid, &st, 0);
    printf("exit status %d\\n", WEXITSTATUS(st));
    return 0;
}
''', sep=sep)
        return {"title": "Run seq in a child", "stars": 2, "summary": "execlp with arguments, then the status of the child.", "statement": statement,
                "hints": ["`execlp(file, argv0, arg1, ..., (char *)NULL)`: the first argument after the file name is the program's own name.",
                          "Code after a successful `exec` never runs; put `_exit(127)` after it for the case in which the command cannot be started."], "tests": tests, "solution": solution}
    opt = r.choice(["-d", "-f", "-e"])
    meaning = {"-d": "is a directory", "-f": "is a regular file", "-e": "exists"}[opt]
    names = {"dir1": "dir", "dir2": "dir", "a.txt": "file", "b.txt": "file"}

    def exists_kind(n):
        return names.get(n)

    def status(n):
        k = exists_kind(n)
        return 0 if (opt == "-e" and k) or (opt == "-d" and k == "dir") or (opt == "-f" and k == "file") else 1
    setup = "mkdir dir1 dir2; touch a.txt b.txt"
    tests = [{"setup": setup, "args": [n], "stdout": f"exit status {status(n)}\n"} for n in ("dir1", "a.txt", "nothing", "b.txt", "dir2")]
    statement = (f"`prog PATH` runs the command `test {opt} PATH` in a child (it succeeds, status 0, if `PATH` {meaning}; otherwise its status is 1). "
                 "The parent waits for the child and prints `exit status S`, where `S` is the exit status of the child.")
    solution = C('''#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    int st;
    pid_t pid;

    if (argc != 2)
        return 2;
    pid = fork();
    if (pid < 0)
        return 1;
    if (pid == 0) {
        execlp("test", "test", "$opt", argv[1], (char *)NULL);
        _exit(127);
    }
    waitpid(pid, &st, 0);
    printf("exit status %d\\n", WEXITSTATUS(st));
    return 0;
}
''', opt=opt)
    return {"title": "Ask test " + opt, "stars": 2, "summary": "Run a command in a child and report its exit status.", "statement": statement,
            "hints": ["`execlp(\"test\", \"test\", \"" + opt + "\", path, (char *)NULL)`.", "`WEXITSTATUS(status)` is the number the command ended with; after a failed `exec` the child should `_exit(127)`."],
            "tests": tests, "solution": solution}


# ------------------------------------------------------------------ pipes


def pipes_total(r):
    kinds = {
        "sum": ("the sum of the numbers", "acc += x;", lambda v: sum(v)),
        "squares": ("the sum of the squares of the numbers", "acc += (long)x * x;", lambda v: sum(x * x for x in v)),
        "evens": ("how many of the numbers are even", "if (x % 2 == 0)\n                acc++;", lambda v: sum(1 for x in v if x % 2 == 0)),
        "positive": ("how many of the numbers are greater than zero", "if (x > 0)\n                acc++;", lambda v: sum(1 for x in v if x > 0)),
    }
    key = r.choice(sorted(kinds))
    what, update, fn = kinds[key]
    tests = []
    for n in (1, 4, r.randint(5, 9), 20):
        v = [r.randint(-50, 99) for _ in range(n)]
        tests.append({"stdin": " ".join(str(x) for x in v) + "\n", "stdout": f"result = {fn(v)}\n"})
    statement = (f"The program reads integers from its standard input until the end of the data, and finds {what}, but **the child process does the work**:\n\n"
                 "- the parent reads each integer with `scanf` and sends it to the child through a pipe (as an `int`, with `write`);\n"
                 "- when the numbers are over the parent closes that pipe; the child, which reads until the end of the data, then sends the result back through a **second** pipe;\n"
                 "- the parent reads it, waits for the child and prints `result = R`.")
    solution = C('''#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
    int up[2], down[2], x;
    long acc = 0;

    if (pipe(up) < 0 || pipe(down) < 0)
        return 1;
    if (fork() == 0) {
        close(up[1]);
        close(down[0]);
        while (read(up[0], &x, sizeof x) == sizeof x) {
            $update
        }
        write(down[1], &acc, sizeof acc);
        _exit(0);
    }
    close(up[0]);
    close(down[1]);
    while (scanf("%d", &x) == 1)
        write(up[1], &x, sizeof x);
    close(up[1]);
    read(down[0], &acc, sizeof acc);
    close(down[0]);
    wait(NULL);
    printf("result = %ld\\n", acc);
    return 0;
}
''', update=update)
    return {"title": "Let the child do the counting", "stars": 3,
            "summary": "Numbers go down one pipe, the answer comes back up another.", "statement": statement,
            "hints": ["Create BOTH pipes before the `fork`. The parent keeps `up[1]` and `down[0]`; the child keeps `up[0]` and `down[1]`; each one closes the other ends.",
                      "The child's loop ends when `read` returns 0, which only happens after the parent has closed `up[1]` (and the child its own copy)."],
            "tests": tests, "solution": solution}


# ------------------------------------------------------------------ signals


def signals_notify(r):
    sig = r.choice(["SIGUSR1", "SIGUSR2"])
    msg = r.choice(["go", "wake up", "ready", "time to work", "starting", "got it"])
    code = r.randint(3, 40)
    statement = (f"The parent installs a handler for `{sig}` **before** creating a child. The child waits (`while (!got) pause();`, where the handler sets `got`) until it receives the signal; "
                 f"then it prints `{msg}` and ends with exit status {code}. A moment after the `fork` (`usleep(100000)`) the parent sends `{sig}` to the child with `kill`, "
                 "waits for it and prints `child exited with S`.")
    solution = C('''#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

static volatile sig_atomic_t got;

static void handler(int sig)
{
    (void)sig;
    got = 1;
}

int main(void)
{
    int st;
    pid_t pid;

    signal($sig, handler);
    pid = fork();
    if (pid < 0)
        return 1;
    if (pid == 0) {
        while (!got)
            pause();
        printf("$msg\\n");
        exit($code);
    }
    usleep(100000);
    kill(pid, $sig);
    waitpid(pid, &st, 0);
    printf("child exited with %d\\n", WEXITSTATUS(st));
    return 0;
}
''', sig=sig, msg=msg, code=code)
    return {"title": "Wake the child with " + sig, "stars": 3, "summary": "A child that waits for one signal, then reports.", "statement": statement,
            "hints": ["The handler only sets a flag; install it with `signal` before the `fork` so the child inherits it.",
                      "The child waits with a loop `while (!got) pause();`; `printf` and `exit` come after it. The parent uses `kill(pid, sig)` and `waitpid`."],
            "tests": [{"args": [], "stdout": f"{msg}\nchild exited with {code}\n"}], "solution": solution}


# ------------------------------------------------------------------ threads


def threads_partial(r):
    forms = {
        "i": ("`i`", "i", lambda i: i),
        "squares": ("`i * i`", "i * i", lambda i: i * i),
        "multiples": ("`i` if `i` is a multiple of 3, else 0", "(i % 3 == 0) ? i : 0", lambda i: i if i % 3 == 0 else 0),
        "parity": ("`i` if `i` is even, else `-i`", "(i % 2 == 0) ? i : -i", lambda i: i if i % 2 == 0 else -i),
    }
    key = r.choice(sorted(forms))
    text, expr, fn = forms[key]
    tests = []
    for n, t in ((10, 2), (100, 4), (r.randint(200, 900), r.randint(2, 6)), (1000, 8), (7, 3)):
        tests.append({"args": [str(n), str(t)], "stdout": f"total = {sum(fn(i) for i in range(1, n + 1))}\n"})
    statement = (f"`prog N T` (with `N` from 1 to 100000 and `T` from 1 to 8) adds up, with `T` threads, the value {text} for `i` from 1 to `N`. "
                 "Thread number `k` (from 0) takes the values of `i` such that `(i - 1) % T == k` and puts its partial sum in a slot of its own of a global array; "
                 "the main thread joins them all, adds the partial sums and prints `total = X`.")
    solution = C('''#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAXT 8

static long n;
static int nthreads;
static long partial[MAXT];

static void *work(void *arg)
{
    long k = (long)arg, i;

    for (i = 1 + k; i <= n; i += nthreads)
        partial[k] += $expr;
    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t th[MAXT];
    long total = 0, k;

    if (argc != 3)
        return 2;
    n = atol(argv[1]);
    nthreads = atoi(argv[2]);
    if (n < 1 || nthreads < 1 || nthreads > MAXT)
        return 2;
    for (k = 0; k < nthreads; k++)
        pthread_create(&th[k], NULL, work, (void *)k);
    for (k = 0; k < nthreads; k++) {
        pthread_join(th[k], NULL);
        total += partial[k];
    }
    printf("total = %ld\\n", total);
    return 0;
}
''', expr=expr)
    return {"title": "Partial sums in threads", "stars": 3, "summary": "T threads, each with a slot of its own: no lock needed.", "statement": statement,
            "hints": ["Pass the thread number as the argument of `pthread_create`: `(void *)k`, and convert it back with `(long)arg`.",
                      "Each thread writes only its own `partial[k]`, so there is no race; add them up after the `pthread_join`s."],
            "tests": tests, "solution": solution}


# ------------------------------------------------------------------ semaphores


def semaphores_turns(r):
    k = r.randint(2, 4)
    names = r.sample(["ping", "pong", "tick", "tock", "red", "green", "blue", "one", "two", "three", "north", "south"], k)
    tests = []
    for n in (1, 2, r.randint(3, 6), 10):
        tests.append({"args": [str(n)], "stdout": "".join(f"{names[j]}\n" for _ in range(n) for j in range(k))})
    statement = (f"`prog N` creates {k} threads that print, in this order and `N` times each, the words {', '.join('`' + x + '`' for x in names)} (one word per line): "
                 f"the whole output is `{names[0]}`, `{names[1]}`" + (", ..." if k > 2 else "") + f" repeated `N` times. The threads must take turns using **semaphores** "
                 "(one per thread); the main thread joins them and prints nothing else.")
    arr = ", ".join(f'"{x}"' for x in names)
    solution = C('''#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define K $k

static const char *word[K] = {$arr};
static sem_t turn[K];
static int rounds;

static void *run(void *arg)
{
    long me = (long)arg;
    int i;

    for (i = 0; i < rounds; i++) {
        sem_wait(&turn[me]);
        printf("%s\\n", word[me]);
        sem_post(&turn[(me + 1) % K]);
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t th[K];
    long i;

    if (argc != 2)
        return 2;
    rounds = atoi(argv[1]);
    for (i = 0; i < K; i++)
        sem_init(&turn[i], 0, i == 0 ? 1 : 0);
    for (i = 0; i < K; i++)
        pthread_create(&th[i], NULL, run, (void *)i);
    for (i = 0; i < K; i++)
        pthread_join(th[i], NULL);
    return 0;
}
''', k=k, arr=arr)
    return {"title": "Take turns: " + " / ".join(names), "stars": 3 if k < 4 else 4, "summary": "A ring of semaphores that passes the turn from thread to thread.", "statement": statement,
            "hints": ["One semaphore per thread: the first starts at 1, the others at 0.",
                      "Thread `i` does `sem_wait` on its own semaphore, prints, and `sem_post`s the semaphore of the next thread (the last one wakes the first)."],
            "tests": tests, "solution": solution}


# ------------------------------------------------------------------ condition variables


def condvars_buffer(r):
    size = r.randint(1, 5)
    forms = {
        "i": ("`i`", "i", lambda i: i),
        "double": ("`2 * i`", "2 * i", lambda i: 2 * i),
        "square_mod": ("`(i * i) % 10`", "(i * i) % 10", lambda i: (i * i) % 10),
    }
    key = r.choice(sorted(forms))
    text, expr, fn = forms[key]
    tests = []
    for n in (1, 5, r.randint(10, 60), 500, 3000):
        tests.append({"args": [str(n)], "stdout": f"sum = {sum(fn(i) for i in range(1, n + 1))}\nbuffer never above {size}: yes\n"})
    statement = (f"`prog N` runs a producer thread and a consumer thread over a **circular buffer of {size} slot{'s' if size > 1 else ''}**. The producer puts the values {text} for `i` from 1 to `N`; "
                 "the consumer takes `N` values and adds them up. Use **one mutex and two condition variables** (`not_full` and `not_empty`), and keep the number of values in the buffer to check "
                 f"that it never goes above {size}. The main thread joins both and prints `sum = S` and then `buffer never above {size}: yes` (or `no`).")
    solution = C('''#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE $size

static int buf[SIZE], in, out, count, max_count, n;
static long sum;
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t not_full = PTHREAD_COND_INITIALIZER;
static pthread_cond_t not_empty = PTHREAD_COND_INITIALIZER;

static void *producer(void *arg)
{
    int i;

    (void)arg;
    for (i = 1; i <= n; i++) {
        pthread_mutex_lock(&lock);
        while (count == SIZE)
            pthread_cond_wait(&not_full, &lock);
        buf[in] = $expr;
        in = (in + 1) % SIZE;
        count++;
        if (count > max_count)
            max_count = count;
        pthread_cond_signal(&not_empty);
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

static void *consumer(void *arg)
{
    int i;

    (void)arg;
    for (i = 1; i <= n; i++) {
        pthread_mutex_lock(&lock);
        while (count == 0)
            pthread_cond_wait(&not_empty, &lock);
        sum += buf[out];
        out = (out + 1) % SIZE;
        count--;
        pthread_cond_signal(&not_full);
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t p, c;

    if (argc != 2 || (n = atoi(argv[1])) < 1)
        return 2;
    pthread_create(&p, NULL, producer, NULL);
    pthread_create(&c, NULL, consumer, NULL);
    pthread_join(p, NULL);
    pthread_join(c, NULL);
    printf("sum = %ld\\n", sum);
    printf("buffer never above %d: %s\\n", SIZE, max_count <= SIZE ? "yes" : "no");
    return 0;
}
''', size=size, expr=expr)
    return {"title": f"Buffer of {size} with conditions", "stars": 4, "summary": "Bounded buffer with one mutex and two condition variables.", "statement": statement,
            "hints": ["The producer waits on `not_full` while `count == SIZE`; the consumer waits on `not_empty` while `count == 0`. Always in a `while` loop.",
                      "After changing `count`, signal the OTHER condition: the producer signals `not_empty`, the consumer `not_full`."],
            "tests": tests, "solution": solution}


# ------------------------------------------------------------------ scheduling (CPU only)


def _sim(procs, policy):
    """Waiting and turnaround times, FCFS or SJF without preemption (ties: arrival, then listing order). procs: [(arrival, burst)]."""
    n = len(procs)
    done = [False] * n
    t = 0
    wait, turn = [0] * n, [0] * n
    for _ in range(n):
        ready = [i for i in range(n) if not done[i] and procs[i][0] <= t]
        if not ready:
            t = min(procs[i][0] for i in range(n) if not done[i])
            ready = [i for i in range(n) if not done[i] and procs[i][0] <= t]
        if policy == "fcfs":
            i = min(ready, key=lambda k: (procs[k][0], k))
        else:
            i = min(ready, key=lambda k: (procs[k][1], procs[k][0], k))
        wait[i] = t - procs[i][0]
        t += procs[i][1]
        turn[i] = t - procs[i][0]
        done[i] = True
    return wait, turn


def scheduling_cpu(r):
    policy, metric = r.choice([("fcfs", "wait"), ("fcfs", "turn"), ("sjf", "wait"), ("sjf", "turn")])
    name = "first come, first served" if policy == "fcfs" else "shortest job first, without preemption"
    word = "waiting time (from its arrival until it starts running)" if metric == "wait" else "turnaround time (from its arrival until it finishes)"
    tests = []
    for n in (1, 3, r.randint(4, 6), 7):
        procs = sorted(((r.randint(0, 12), r.randint(1, 12)) for _ in range(n)), key=lambda p: p[0])
        w, t = _sim(procs, policy)
        vals = w if metric == "wait" else t
        out = "".join(f"P{i + 1}: {v}\n" for i, v in enumerate(vals)) + f"average: {sum(vals) / n:.2f}\n"
        tests.append({"stdin": f"{n}\n" + "".join(f"{a} {b}\n" for a, b in procs), "stdout": out})
    statement = (f"Simulate **{name}** scheduling with no I/O (each process has one CPU burst) and print the **{word}** of every process.\n\n"
                 "**Input** (standard input): the number of processes `N` (from 1 to 50), then `N` lines with the **arrival time** and the **duration** of each, in whole milliseconds, "
                 "listed in order of arrival. The processes are called `P1`, `P2`, ... in the order they are listed.\n\n"
                 "**Output**: one line `Pk: V` per process and then `average: A` (two decimals). "
                 + ("When the CPU is free it takes the process that has been waiting longest (ties: the one listed first)." if policy == "fcfs" else
                    "When the CPU is free it takes the ready process with the shortest duration (ties: the one that arrived first, then the one listed first)."))
    solution = C('''#include <stdio.h>

#define MAX 50

struct proc {
    int arr, dur, done, value;
};

int main(void)
{
    struct proc p[MAX];
    int n, i, k, best, t = 0;
    long total = 0;

    if (scanf("%d", &n) != 1 || n < 1 || n > MAX)
        return 1;
    for (i = 0; i < n; i++) {
        if (scanf("%d %d", &p[i].arr, &p[i].dur) != 2)
            return 1;
        p[i].done = 0;
    }
    for (k = 0; k < n; k++) {
        best = -1;
        while (best < 0) {
            for (i = 0; i < n; i++)
                if (!p[i].done && p[i].arr <= t && (best < 0 || $better))
                    best = i;
            if (best < 0)
                t++;   /* nobody has arrived yet: the CPU is idle */
        }
        $value
        t += p[best].dur;
        p[best].done = 1;
    }
    for (i = 0; i < n; i++) {
        printf("P%d: %d\\n", i + 1, p[i].value);
        total += p[i].value;
    }
    printf("average: %.2f\\n", (double)total / n);
    return 0;
}
''', better=("p[i].arr < p[best].arr" if policy == "fcfs" else "p[i].dur < p[best].dur || (p[i].dur == p[best].dur && p[i].arr < p[best].arr)"),
        value=("p[best].value = t - p[best].arr;" if metric == "wait" else "p[best].value = t + p[best].dur - p[best].arr;"))
    return {"title": ("FCFS" if policy == "fcfs" else "SJF") + (": waiting times" if metric == "wait" else ": turnaround times"), "stars": 3,
            "summary": "A CPU-only scheduling simulation with a given order and metric.", "statement": statement,
            "hints": ["Keep the current time `t`. Repeat `N` times: among the processes that have arrived and are not done, choose one by the rule, and let it run to completion.",
                      "If nobody has arrived yet, jump `t` to the earliest arrival. Waiting time is `start - arrival`; turnaround is `finish - arrival`."],
            "tests": tests, "solution": solution}


TEMPLATES = [
    ("t4-tpl-procs-children", "procs", procs_children),
    ("t4-tpl-exec-wrapper", "exec", exec_wrapper),
    ("t4-tpl-pipes-total", "pipes", pipes_total),
    ("t4-tpl-signals-notify", "signals", signals_notify),
    ("t4-tpl-threads-partial", "threads", threads_partial),
    ("t4-tpl-semaphores-turns", "semaphores", semaphores_turns),
    ("t4-tpl-condvars-buffer", "condvars", condvars_buffer),
    ("t4-tpl-scheduling-cpu", "scheduling", scheduling_cpu),
]

# Minutes a student takes to write and pass one exercise of the template (an estimate, compared with the course exercises of the same chapter).
MINUTES = {
    "t4-tpl-procs-children": 8, "t4-tpl-exec-wrapper": 7, "t4-tpl-pipes-total": 14, "t4-tpl-signals-notify": 11,
    "t4-tpl-threads-partial": 12, "t4-tpl-semaphores-turns": 12, "t4-tpl-condvars-buffer": 18, "t4-tpl-scheduling-cpu": 16,
}
