# The most frequent word

`topword FILE` finds the most frequent word of the file and prints `<word> <count>`. A **word** is a maximal run of letters (a-z, A-Z); everything else separates words; upper and lower case are the same word and the result is printed in lower case. If several words tie, print the one that comes first alphabetically. A file without words prints `no words`. Missing file: `cannot open FILE` on standard error, exit 1.

(Words are at most 60 letters; the file can contain thousands of different words.)

**Example**

```
$ ./prog f.txt
the 3
```
