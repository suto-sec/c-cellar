# Number the lines

`numberlines FILE` prints every line of the file preceded by its number, right-aligned in 4 characters, followed by two spaces (like `cat -n` with a different width). Lines of the file never exceed 200 characters and the file ends with a newline.

**Example**

```
$ ./prog f.txt
   1  a
   2  b
```
