# filosofos: with semaphores

Five philosophers sit at a round table with five chopsticks, one between each pair. Philosopher `i` (0 to 4) needs chopsticks `i` and `(i + 1) % 5` to eat.

The program creates **5 semaphores** (one per chopstick) and **5 threads** (the philosophers) that run the same function. Each philosopher repeats **3 times**: think (nothing to do), pick up both chopsticks, eat (add 1 to its own meal counter), put them down. The program must be **free of deadlock**.

**Output**: the philosophers print **nothing** while they run. After joining all the threads, the main thread prints one line per philosopher, in order, `philosopher I ate 3 times`, and a last line `total meals: 15`.

A philosopher that never gets both chopsticks makes the program hang, so a wrong solution is caught by the time limit.

**Example**

```
$ ./prog
philosopher 0 ate 3 times
philosopher 1 ate 3 times
philosopher 2 ate 3 times
philosopher 3 ate 3 times
philosopher 4 ate 3 times
total meals: 15
```
