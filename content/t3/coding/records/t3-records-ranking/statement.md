# Class ranking

Write `ranking FILE`. Each line of the file is a record `NAME GRADE`: a name without spaces (at most 31 characters) and a grade (a number that can have decimals). Print the students ordered by grade, **highest first** (if two grades are equal, by name in `strcmp` order), one per line as `POSITION. NAME GRADE` with the grade having two decimals and positions 1, 2, 3... (equal grades still get different positions). After them print `average: <mean>` with two decimals.

- Blank lines are ignored. The file can have any number of students.
- A line that is not a record prints `ranking: FILE: line N: bad record` to standard error and the program exits with status 1 without printing the list (`N` counts all the lines, from 1).
- A file with no students prints `ranking: FILE: no students` to standard error and exits with status 1.
- A file that cannot be opened: `ranking: FILE: <reason>` (`strerror(errno)`) to standard error and status 1. Wrong number of arguments: `usage: ranking FILE` and status 2.

**Example**

```
$ cat grades.txt
ana 7.5
bob 9
carl 7.5
$ ./prog grades.txt
1. bob 9.00
2. ana 7.50
3. carl 7.50
average: 8.00
```
