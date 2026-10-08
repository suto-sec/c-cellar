# A small tee

`teecopy FILE` copies everything it reads from **standard input** (descriptor 0) to **standard output** and also to `FILE` (created or emptied, 0644). Use `read` and `write` only, with a buffer of 256 bytes.

**Example**

```
$ printf 'abc' | ./prog copy.txt; echo; cat copy.txt
abc
abc```
