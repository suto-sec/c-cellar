# c-cellar

A self-hosted lab to practise C. It runs on your own computer, in a container, and you use it in your browser: a statement, an editor (VS Code) and a terminal side by side, and a **Check** button that compiles your program, runs it against test cases and shows what went wrong.

- **209 coding exercises** in 19 chapters (output, variables, operators, input, conditions, loops, functions, arrays, strings, pointers, dynamic memory, structs, arguments, files, system calls, directories, errors, modules and make, exam-style programs), each from 1 to 5 stars
- **A new exercise every day** (the daily challenge) with a streak
- **348 theory questions** in 7 formats, every answer explained
- **A reference** of 146 entries: every function, command and keyword the course uses

Everything you do is stored in the `.progress/` folder of the project (delete it to start over). Nothing leaves your computer.

## Contents

- [Install](#install): [the installer scripts](#quickest-the-installer-scripts) · [Linux](#linux-by-hand) · [macOS](#macos-by-hand) · [Windows](#windows-by-hand) · [daily use](#daily-use) · [troubleshooting](#if-something-goes-wrong)
- [Using it](#using-it)
- [How it is organised](#how-it-is-organised) (for people who want to change or extend it)
- [License](#license)

---

## Install

> **Tested on Linux with podman only.** The image, the container and `install/linux.sh` were built and run on Linux with podman. Docker, macOS and Windows (WSL)
> use the same `./lab` script and are described below, but **they have not been tested**: `install/macos.sh` and `install/windows.ps1` have never been run on
> a Mac or a Windows machine. If something fails there, open an issue with the exact error text.

You need three things: **a container engine** (podman or Docker), **git**, and **a Linux-style terminal**. The installer scripts do it for you; the step-by-step guides do it by hand.

| your system | what to install | where you type the commands |
|---|---|---|
| **Linux** | podman (or Docker), git, curl | any terminal |
| **macOS** | Docker Desktop, git | the Terminal app |
| **Windows 10/11** | WSL 2 with Ubuntu, and podman inside it (or Docker Desktop) | the installer runs in **PowerShell**; by hand, everything runs in **the Ubuntu terminal** (not Git Bash, not PowerShell) |

Disk space: about **4 GB free** (the image is about 2 GB). The first start downloads and builds it and takes **5 to 15 minutes**; every start after that takes a few seconds.

### Quickest: the installer scripts

The `install/` folder has one script per system. Each one checks what you already have, **asks before it installs anything**, installs what is missing, downloads the project (if you do not have it yet) and starts the lab. You can run it again at any time: it skips what is done.

| system | run this | what it does |
|---|---|---|
| **Linux** (Debian/Ubuntu/Mint, Fedora/RHEL, Arch/Manjaro, openSUSE) | `curl -fsSL https://raw.githubusercontent.com/suto-sec/c-cellar/main/install/linux.sh \| bash` | installs podman (or Docker with `--docker`), git, curl and the rootless-podman settings with your package manager; downloads the project to `~/c-cellar`; starts the lab |
| **macOS** | `curl -fsSL https://raw.githubusercontent.com/suto-sec/c-cellar/main/install/macos.sh \| bash` | installs git (command line tools) and Docker Desktop (with Homebrew, which it can install too), waits for Docker, downloads the project, starts the lab |
| **Windows** (PowerShell) | `& ([scriptblock]::Create((irm https://raw.githubusercontent.com/suto-sec/c-cellar/main/install/windows.ps1)))` | installs WSL and Ubuntu 24.04 (one restart), then podman/git/curl **inside** Ubuntu, downloads the project there and starts the lab (`-Docker` uses Docker Desktop instead) |

Already have the project? Run `./install/linux.sh`, `./install/macos.sh` or `powershell -ExecutionPolicy Bypass -File install\windows.ps1`.
All of them accept `--help` (Linux/macOS) or `Get-Help .\install\windows.ps1` (Windows), and `--dry-run` / `-DryRun` shows what would happen without changing anything. The scripts are short and in plain text: read one before you run it if you like.

> Status: `install/linux.sh` was run on Linux with podman; its choices for the other distributions were only checked with dry runs. `install/macos.sh` and
> `install/windows.ps1` have not been run on a Mac or a Windows machine. If one fails, the step-by-step guides below do the same by hand.

### Linux (by hand)

1. Open a terminal and install the tools (choose your distribution):

   ```bash
   # Ubuntu 24.04 / Debian 12 and newer
   sudo apt update && sudo apt install -y podman git curl

   # Fedora
   sudo dnf install -y podman git curl

   # Arch / Manjaro
   sudo pacman -S --needed podman git curl
   ```

   Podman must be **version 4.3 or newer** (`podman --version`). On older systems (for example Ubuntu 22.04) use Docker instead:
   `sudo apt install -y docker.io git curl && sudo usermod -aG docker $USER`, then **log out and in again**.
2. Download the project and start it:

   ```bash
   git clone https://github.com/suto-sec/c-cellar.git
   cd c-cellar
   ./lab web
   ```

3. Wait until it prints `c-cellar is running at http://localhost:8080`. Your browser opens by itself; if it does not, open that address.

### macOS (by hand)

1. Install **Docker Desktop** from <https://www.docker.com/products/docker-desktop/> (or `brew install --cask docker`), start it and wait until the whale icon in the menu bar stops moving. Git comes with the Xcode command line tools (`xcode-select --install`).
2. In the Terminal app:

   ```bash
   git clone https://github.com/suto-sec/c-cellar.git
   cd c-cellar
   ./lab web
   ```

3. Wait for `c-cellar is running at http://localhost:8080`. The browser opens by itself; otherwise open that address.

### Windows (by hand)

Windows cannot run the `./lab` script directly, so you use **WSL** (the Windows Subsystem for Linux, built into Windows 10/11). All the commands below are typed in the **Ubuntu** terminal, never in PowerShell or Git Bash.

1. **Install WSL with Ubuntu.** Open **PowerShell as administrator** (right-click Start, then *Terminal (Admin)*) and run:

   ```powershell
   wsl --install -d Ubuntu-24.04
   ```

   Restart the computer when it asks. Then open **Ubuntu** from the Start menu; the first time it asks you to invent a Linux username and password (the password is not shown while you type; that is normal).
2. **Install the container engine.** Choose one:
   - *Simplest:* install **Docker Desktop** for Windows (<https://www.docker.com/products/docker-desktop/>), start it, open *Settings, Resources, WSL integration*, switch **Ubuntu-24.04** on and press *Apply*.
   - *Without Docker Desktop:* in the Ubuntu terminal run `sudo apt update && sudo apt install -y podman git curl`.
3. In the **Ubuntu terminal** download the project **into your Linux home folder** and start it:

   ```bash
   cd ~
   git clone https://github.com/suto-sec/c-cellar.git
   cd c-cellar
   ./lab web
   ```

   Do **not** put the project under `/mnt/c/...` (your Windows drives): it is very slow there and breaks permissions.
4. When it prints `c-cellar is running at http://localhost:8080`, open that address in your normal Windows browser (WSL forwards `localhost`).

> **Windows vs Linux, in short:** after the install, everything is identical: same commands, same screens, same files. The only difference is *getting there*: on Linux `./lab` runs natively; on Windows you first create a Linux environment with WSL and run everything inside it; on macOS you use Docker Desktop and the Terminal app. `./lab` detects Git Bash/Cygwin and tells you to use WSL.

### Daily use

Type these in the project folder.

| you want to | type |
|---|---|
| start the lab | `./lab web` |
| stop it (your progress is kept) | `./lab stop` |
| a shell in the lab environment (gcc, gdb, valgrind, `man`) | `./lab shell` |
| check that every exercise and all content is consistent | `./lab selftest` |
| see the logs | `./lab logs` |
| use other ports (if 8080, 8081 or 8082 are taken) | `LAB_PORT=8090 LAB_VSCODE_PORT=8091 LAB_TERM_PORT=8092 ./lab web`, then open `http://localhost:8090` |
| force an engine | `LAB_ENGINE=docker ./lab web` |
| update to the newest version | `git pull`, then `./lab stop`, `./lab build`, `./lab web` |
| start from zero | `./lab stop`, then delete the `.progress/` folder |
| remove everything | `./lab stop`, `podman rmi c-cellar` (or `docker rmi c-cellar`), then delete the project folder |

### If something goes wrong

| what you see | what to do |
|---|---|
| `c-cellar needs podman or docker` | the engine is not installed (or not in this terminal): redo the install for your system. On Windows, run it in the Ubuntu terminal and enable WSL integration in Docker Desktop. |
| `permission denied: ./lab` | the file lost its executable bit (it happens when the project was downloaded as a ZIP): run `chmod +x lab` or start with `bash lab web`. |
| `Cannot connect to the Docker daemon` | Docker is not running: start Docker Desktop (Windows/macOS) or `sudo systemctl start docker` (Linux). On Linux also check that `groups` shows `docker` (log out and in after `usermod`). |
| podman: `no subuid ranges found` / `cannot find newuidmap` | rootless podman needs: `sudo apt install uidmap` and `sudo usermod --add-subuids 100000-165535 --add-subgids 100000-165535 $USER`, then `podman system migrate`. |
| `address already in use` / port 8080 busy | use other ports: `LAB_PORT=8090 LAB_VSCODE_PORT=8091 LAB_TERM_PORT=8092 ./lab web`. |
| `bad interpreter` or `\r: command not found` | the files got Windows line endings. In WSL: `git config --global core.autocrlf false`, delete the folder, clone again **inside WSL**. (The repository forces Unix line endings, so a fresh clone is fine.) |
| the page is blank, or the editor or terminal pane says it cannot connect, right after `./lab web` | wait about 30 seconds and reload: VS Code and the terminal are still starting. |
| the first start seems stuck | the image is being built (5 to 15 minutes). `podman ps` / `docker ps` shows `c-cellar` when it is done. |
| `Permission denied` in the container on Fedora and friends (SELinux) | the script already mounts the project with the `:Z` label; if it still fails, run `./lab stop` and start again, and make sure the project is in your home folder. |
| a browser tab opens on the wrong address | the address is always the one printed by `./lab web`; it is `http://localhost:8080` unless you set `LAB_PORT`. |
| anything else | `./lab stop` and `./lab web` again. Logs: `./lab logs`. |

The ports listen on `127.0.0.1` (your own computer) only. The editor and the terminal have no password because other machines cannot reach them: do not publish those ports on a shared network. Your programs run with a time limit and CPU, memory and file-size limits in a throw-away folder, but that is **not** a security sandbox: run only code you wrote.

---

## Using it

Open `http://localhost:8080`. The top bar has **Coding**, **Theory**, **Daily challenge**, **Reference**, **Layout** and **Settings**.

### Coding exercises

1. Open **Coding**, pick a chapter (or press *Continue* on the home page, which takes you to the next exercise you have not passed) and open an exercise.
2. Read the statement. Exercises start from a **blank file**: you write the whole program in `answer.c`, in VS Code or in the terminal. Both edit the same file; the terminal starts in the exercise's folder.
3. Press **Check** (`Ctrl+Enter`). Your program is compiled with `gcc -Wall -Wextra`, run against the test cases, and each failing case shows what was run and *expected* against *yours*.
4. Stuck? **Hint** gives clues one at a time. **Recommended commands** lists the reference entries of the functions and tools involved, without saying exactly how to use them. **Answer** shows the reference solution (the exercise is then marked *solution viewed* until you pass it).
5. **Previous** and **Next** move through the chapter. The coding page filters by topic and difficulty and remembers its filters.

The **Layout** button arranges the statement, the terminal and VS Code: pick a prebuilt layout, or drag the panels by their headers to put them where you want. The layout is remembered.

### Daily challenge

Every day the app builds **one new exercise** from a template of the course topics (different numbers, words and tasks each day, the same kind of exercise never comes back within five days). Open **Daily challenge** in the top bar: today's exercise is there, and so are all the past ones (press *Retry* to do an old one again).

The **streak** counts consecutive days on which you passed that day's exercise **on that same day**. The flame next to *Daily challenge* is lit only when today's exercise is done. If it distracts you, switch it off in **Settings, Top-bar streak**.

### Theory

Questions in 20 sets that mirror the coding chapters, easiest first, in seven formats: single choice, multiple choice, fill in the blank (also inside a code listing), drag to reorder, predict the output of a program, match pairs and sort into categories. Every answer comes with an explanation; where the C standard and what a course quiz usually expects differ, the explanation says so.

### Reference

One entry per function, command and keyword, grouped by category (each category folds with its arrow; clicking its name lists only that category), with a search that puts the entry called exactly what you typed first. Press the star on an entry to make it a favorite; favorites are listed when nothing is selected. Click the selected entry again, or *Back to Reference*, to return to the list. Every entry lists the theory questions and the exercises that use it.

### Topics, filters and settings

All content is tagged by course topic. Today there is only **`T3`** (tema 3); more topics will be added as new tags. Every exercise, theory set and reference entry shows its tag, and each page has a **Topic** filter (the coding and theory pages also a **Difficulty** filter; several chips can be on at once, none means all).

**Settings** has the theme (light, dark, system), the **accent colour** (a colour picker, or any CSS colour such as `#b4542b`, `rgb(180 84 43)` or `tomato`) and the top-bar streak.

---

## How it is organised

```
lab                      launcher (podman or docker)
install/                 installer scripts for Linux, macOS and Windows
container/               Containerfile (gcc, gdb, valgrind, man pages, ttyd, code-server), entrypoint, terminal startup
app/server.py            backend: content loader, exercise checker, grading, progress (Python, standard library only)
app/daily.py             the daily challenge: builds one exercise a day from a template, and the streak
app/static/              the web interface (plain JS, no build step)
app/selftest.py          consistency checks used by `./lab selftest`
content/<tag>/           everything you study, per topic tag
  chapters.json, path.json   the chapters and the suggested path
  coding/<chapter>/<id>/     one folder per exercise
  theory/<chapter>.json  one question set per chapter
  reference/<area>.json  reference entries
  daily.py               the templates of the daily challenge
.progress/               your progress, your answers, VS Code settings (git-ignored)
```

Every content id starts with its tag (`t3-loops-factorial`, `t3-ptr-04`, `t3-ref-malloc`) and lives under `content/<tag>/`; `selftest` enforces both.

### An exercise (`content/t3/coding/<chapter>/<id>/`)

| file | |
|---|---|
| `meta.json` | `id` (`t3-<chapter>-<slug>`), `tag`, `track` (`derusting` or `exercises`, the same as its chapter), `topic` (the chapter id), `title`, `stars` 1-5, `order` (inside the chapter), `summary`, `hints[]` (at least 2) |
| `statement.md` | what to write |
| `solution.c` | reference solution (shown by *Answer*; `selftest` proves it passes) |
| `tests.json` | `{"cases": [...]}`, see below |
| `harness.c`, `*.h` | optional: a hidden `main` linked with the student's `answer.c`; headers are also copied next to `answer.c` |
| `fixtures/` | optional files copied into every test's working directory |

The student's file starts **blank** (`answer.c`). A multi-file exercise lists its files in `meta.json` (`"files": ["util.h", "util.c", "main.c"]`) and a build command (`"build": "gcc -o prog main.c util.c"` or `"make"`); its reference solution is the folder `solution/` and files the student is linked with go in `hidden/`. Every test then runs in a copy of the finished build directory.

`content/<tag>/chapters.json` lists the chapters; `content/<tag>/path.json` is the suggested path (every exercise exactly once; `selftest` checks that the stars never jump).

A test case: `name`, then either `args` (+ optional `stdin`) or a shell `cmd` (`$BIN` is the compiled program), optional `setup` (shell, run first), `stdout` (expected, compared with `match`: `exact` (default), `trim`, `sorted`, `regex`, `contains`), `exit` (`0` by default, or an integer, `"nonzero"`, `"any"`), `stderr_empty`, `timeout`, `hidden`. Expected outputs must come from the statement, never from running the solution.

### Reference entries (`content/t3/reference/*.json`)

`{"order": n, "entries": [...]}`; an entry has `id` (`t3-ref-<name>`), `title` (the name you would type), `category`, `summary`, and optionally `syntax`, `header`, `description`, `details[]` (`name`/`text`: options, flags, formats), `example`, `mistakes[]`, `see[]` and `aliases[]` (other names that lead here: `%zu`, `O_CREAT`, `else`). Exercises that use an entry are found by name in the reference solutions (the title, plus aliases that look like code: constants, conversions, headers, types, keywords) and, for the tools of *Building programs*, in the build and test commands; a concept entry such as `pointers` or `operators` instead names the coding chapters that teach it in `practice_chapters[]`. A name or alias belongs to one entry only. `selftest` fails if a library function used by a reference solution has no entry.

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

### Daily challenge templates (`content/t3/daily.py`)

`TEMPLATES` is a list of `(id, chapter, function)`. The function gets a `random.Random` and returns a spec: `title`, `stars`, `summary`, `statement`, `hints`, `tests`, and `solution` (or `solution_files`), optionally `harness`, `headers`, `hidden`, `files`, `build`, `example`. The expected output of every test is computed in Python, never by running the solution. The server picks the day's template from the date (never one used in the last five days), generates the spec once and writes it as an ordinary exercise folder in `.progress/daily/`, so the checker, the editor and *Answer* work as for any other exercise. `selftest` builds six seeds of every template and checks that the reference solution compiles without warnings and passes, and that an empty answer fails.

## License

[PolyForm Noncommercial 1.0.0](LICENSE): you may use, copy and change c-cellar for any noncommercial purpose (studying, teaching, research, hobby projects); you may not use it commercially.
