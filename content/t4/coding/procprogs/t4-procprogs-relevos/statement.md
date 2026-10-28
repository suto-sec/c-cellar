# relevos: a relay race

Write a program that simulates a **relay race** with four child processes ("runners"):

1. The parent creates four children, runners 1 to 4. Each one waits for a signal (`SIGUSR1`) to start running.
2. When all are created, the parent prints `all runners created`, waits one second with `sleep(1)` (a signal that reaches a process before it is ready to receive it can be lost, so the runners need time to get to their `pause`), prints `go!` and signals runner 1.
3. A runner that receives the signal prints `runner K running` and then **passes the baton** to the next runner: it prints `runner K passes the baton to runner K+1`, signals it and ends. The last runner prints `runner 4 finished the race` and ends.
4. The parent waits for all the runners and prints `all runners finished`.

(The original exercise also has every runner `sleep` to pretend to run; you do not need that here.) Do not assume the pids of the children are consecutive.

The output must always be exactly:

```
all runners created
go!
runner 1 running
runner 1 passes the baton to runner 2
runner 2 running
runner 2 passes the baton to runner 3
runner 3 running
runner 3 passes the baton to runner 4
runner 4 running
runner 4 finished the race
all runners finished
```
