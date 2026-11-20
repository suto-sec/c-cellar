# robots: pieces on a shared tray

Three robot threads share a tray.

- **R1** makes pieces of type T1 and puts them on the tray, **R2** makes pieces of type T2. The tray has room for **two T1 and two T2**. A robot that finds no room for its piece waits.
- **R3** needs **two T1 and two T2** to make one piece of type T3. It takes the four pieces from the tray at once, and only when all four are there. Otherwise it waits.

`prog N` runs the robots until R3 has made `N` pieces of type T3. Then every thread must end (R1 and R2 may be waiting at that moment!) and the main thread, after joining them, prints:

```
T3 made: N
tray limits respected: yes
pieces used: T1=A T2=B
```

`tray limits respected` is `yes` if the tray never held more than two pieces of any type. `A` and `B` are the pieces R3 took from the tray (`2N` each).

**Example**

```
$ ./prog 500
T3 made: 500
tray limits respected: yes
pieces used: T1=1000 T2=1000
```
