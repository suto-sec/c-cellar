# c-cellar

A self-hosted lab to practise C. It runs in a container and you use it in your browser:

- **Coding exercises**: the statement on the left, VS Code on the right and a **Check** button that compiles your program with `gcc -Wall -Wextra`, runs it against test cases and shows a diff when something is wrong. **Hint** gives clues one at a time, **Answer** shows the reference solution. Exercises start from a blank file: you write the whole program.
  - 19 coding **chapters** in teaching order (output, variables, operators, input, conditions, loops, functions, arrays, strings, pointers, dynamic memory, structs, arguments, stdio files, system calls, directories, errors, modules/libraries/make, exam-style programs), each from 1 to 5 stars.
  - A **suggested path** interleaves all the exercises so the difficulty climbs gradually; *Continue* takes you to the next one you have not passed.
  - *C derusting* are the 18 topic chapters; *C exercises* are the exam-style programs.
- **Theory**: 340+ questions in 20 sets that mirror the coding chapters (plus an introduction to the toolchain), easiest first. Seven formats: single choice, multiple choice, fill in the blank (also inside a code listing), drag to reorder, predict the output of a program, match pairs and sort into categories. Every answer comes with an explanation; where the C standard and what a course quiz usually expects differ, the explanation says so.
- **Cross links**: a reference entry lists the theory questions about it and the exercises that use it; a question links to its reference entries and to the exercises of its chapter; an exercise links to the theory set of its chapter, and its answer to the reference entries of the functions it uses.
- **Reference**: one entry per function, command and keyword, grouped by category, with a search that puts the entry called exactly what you typed first.

Everything you do is stored in the `.progress/` folder (delete it to start over). Nothing leaves your computer.

All content is tagged by course topic. Today there is only **`T3`** (tema 3); more topics will be added as new tags. Every exercise, theory set and reference entry shows its topic tag, and the coding, theory and reference pages each have a **Topic** filter (the coding and theory pages also a **Difficulty** filter; several chips can be on at once, none means all). Each page remembers its own filters.

## Run it

You need **podman** (rootless, preferred) or **docker**, and `bash`. On Arch: `sudo pacman -S podman`.

```bash
git clone https://github.com/suto-sec/c-cellar.git
cd c-cellar
./lab web          # first run builds the image (a few minutes) and opens http://localhost:8080
```

| you want to | type |
|---|---|
| start | `./lab web` |
| stop (progress is kept) | `./lab stop` |
| a shell in the lab environment (gcc, gdb, valgrind, `man`) | `./lab shell` |
| check that every exercise and all content is consistent | `./lab selftest` |
| see the logs | `./lab logs` |
| rebuild the image | `./lab build` |
| use other ports | `LAB_PORT=8090 LAB_VSCODE_PORT=8091 ./lab web` |
| force an engine | `LAB_ENGINE=docker ./lab web` |

Both ports listen on `127.0.0.1` only. The editor has no password because it is not reachable from other machines; do not publish those ports on a shared network. Student programs run with a time limit and CPU, memory and file-size limits in a throw-away folder, but it is **not** a security sandbox: run only code you wrote.

Installation notes for macOS and Windows will be added later; the launcher is plain bash and the image is plain Ubuntu 24.04.

## How it is organised

```
lab                      launcher (podman or docker)
container/               Containerfile (gcc, gdb, valgrind, man pages, code-server) and entrypoint
app/server.py            backend: content loader, exercise checker, grading, progress (Python, standard library only)
app/static/              the web interface (plain JS, no build step)
app/selftest.py          consistency checks used by `./lab selftest`
content/<tag>/           everything you study, per topic tag
  chapters.json, path.json   the chapters and the suggested path
  coding/<chapter>/<id>/     one folder per exercise
  theory/<chapter>.json  one question set per chapter
  reference/<area>.json  reference entries
.progress/               your progress, your answers, VS Code settings (git-ignored)
```

Every content id starts with its tag (`t3-loops-factorial`, `t3-ptr-04`, `t3-ref-malloc`) and lives under `content/<tag>/`; `selftest` enforces both.

### An exercise (`content/t3/coding/<chapter>/<id>/`)

| file | |
|---|---|
| `meta.json` | `id` (`t3-<chapter>-<slug>`), `tag`, `track` (`derusting` or `exercises`, the same as its chapter), `topic` (the chapter id), `title`, `stars` 1-5, `order` (inside the chapter), `summary`, `hints[]` (at least 2), optional `info` |
| `statement.md` | what to write |
| `solution.c` | reference solution (shown by *Answer*; `selftest` proves it passes) |
| `tests.json` | `{"cases": [...]}`, see below |
| `harness.c`, `*.h` | optional: a hidden `main` linked with the student's `answer.c`; headers are also copied next to `answer.c` |
| `fixtures/` | optional files copied into every test's working directory |

The student's file starts **blank** (`answer.c`). A multi-file exercise lists its files in `meta.json` (`"files": ["util.h", "util.c", "main.c"]`) and a build command (`"build": "gcc -o prog main.c util.c"` or `"make"`); its reference solution is the folder `solution/` and files the student is linked with go in `hidden/`. Every test then runs in a copy of the finished build directory.

`content/<tag>/chapters.json` lists the chapters; `content/<tag>/path.json` is the suggested path (every exercise exactly once; `selftest` checks that the stars never jump).

A test case: `name`, then either `args` (+ optional `stdin`) or a shell `cmd` (`$BIN` is the compiled program), optional `setup` (shell, run first), `stdout` (expected, compared with `match`: `exact` (default), `trim`, `sorted`, `regex`, `contains`), `exit` (`0` by default, or an integer, `"nonzero"`, `"any"`), `stderr_empty`, `timeout`, `hidden`. Expected outputs must come from the statement, never from running the solution.

### Reference entries (`content/t3/reference/*.json`)

`{"order": n, "entries": [...]}`; an entry has `id` (`t3-ref-<name>`), `title` (the name you would type), `category`, `summary`, and optionally `syntax`, `header`, `description`, `details[]` (`name`/`text`: options, flags, formats), `example`, `mistakes[]`, `see[]` and `aliases[]` (other names that lead here: `%zu`, `O_CREAT`, `else`). A name or alias belongs to one entry only. `selftest` fails if a library function used by a reference solution has no entry.

### Theory questions (`content/t3/theory/*.json`)

A set has `id` (`t3-th-<chapter>`), `tag`, `title`, `chapter` (the chapter it mirrors) and `questions[]`. Each question has `id`, `type`, `prompt`, `explain`, `difficulty` (1-5 stars), optional `ref[]` (reference names or ids it links to) and optional `code`, and by type:

- `single`: `options[]`, `answer` (index), optional `option_notes[]`
- `multiple`: `options[]`, `answer` (list of indices), optional `option_notes[]`
- `fill`: `{{0}}`, `{{1}}`… markers in `prompt` and/or `code` (shown as inline boxes) and `blanks[]` (each a list of accepted answers, or `{"accept": [...], "ignore_space": true, "case": true}`; an answer starting with `re:` is a regular expression)
- `order`: `items[]` in the CORRECT order (the interface shuffles them)
- `predict`: `code` is a complete program, `answer` is exactly what it prints (a list of alternatives is allowed); trailing spaces and blank lines are ignored. `selftest` compiles and runs every one and compares
- `match`: `pairs[]` of `[left, right]`, each right side used once
- `sort`: `categories[]` and `items[]` of `[item, category]`

A question whose answer depends on what a program prints can carry `verify: {"source": "<C program>", "stdout": "..."}`; `selftest` compiles and runs it, so the explanation cannot drift from the truth.
