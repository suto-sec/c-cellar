"""Daily exercise templates of T3 (see app/daily.py). Each template gets a random.Random and returns a spec:
title, stars, summary, statement, hints, tests (+ solution or solution_files, harness, headers, hidden, files, build, example).
Expected outputs are computed here in Python from the statement, never by running the solution. Only the C of the T3 syllabus is used."""
import math
import shlex
from string import Template

WORDS = ["apple", "river", "stone", "cloud", "tiger", "maple", "ocean", "pixel", "amber", "lemon", "raven", "delta", "ember", "flint", "grove", "haze"]


def C(src, /, **kw):
    """C source from a $name template (C has no $, so this is safe with braces)."""
    return Template(src).substitute(**kw)


def lines_setup(lines, name="data.txt"):
    """Shell that creates a text file with these lines (words of [a-z0-9 -] only: they go through the shell)."""
    return "printf '%s\\n' " + " ".join(shlex.quote(x) for x in lines) + f" > {name}" if lines else f": > {name}"


# ------------------------------------------------------------------ output


def output_frame(r):
    text = " ".join(r.sample(WORDS, 2))
    ch = r.choice("*#=-+~")
    line = ch * len(text)
    return {
        "title": "Framed text", "stars": 1, "summary": "Print a line of text between two border lines.",
        "statement": f"Print the text `{text}` framed: a line made only of `{ch}` characters (exactly as many as the text has characters) above it and another one below it.",
        "hints": ["`printf` prints text; `\\n` ends the line.", f"The text has {len(text)} characters, so each border line has {len(text)} of `{ch}`."],
        "tests": [{"args": [], "stdout": f"{line}\n{text}\n{line}\n"}],
        "solution": C('''#include <stdio.h>

int main(void)
{
    printf("$line\\n");
    printf("$text\\n");
    printf("$line\\n");
    return 0;
}
''', line=line, text=text),
    }


# ------------------------------------------------------------------ input


def input_stats(r):
    kinds = {
        "sum": ("the sum of the numbers", "sum", lambda v: f"sum = {sum(v)}",
                '    long total = 0;\n    for (int i = 0; i < n; i++) {\n        int x;\n        if (scanf("%d", &x) != 1)\n            return 1;\n        total += x;\n    }\n    printf("sum = %ld\\n", total);\n'),
        "max": ("the largest number", "max", lambda v: f"max = {max(v)}",
                '    int best = 0;\n    for (int i = 0; i < n; i++) {\n        int x;\n        if (scanf("%d", &x) != 1)\n            return 1;\n        if (i == 0 || x > best)\n            best = x;\n    }\n    printf("max = %d\\n", best);\n'),
        "min": ("the smallest number", "min", lambda v: f"min = {min(v)}",
                '    int best = 0;\n    for (int i = 0; i < n; i++) {\n        int x;\n        if (scanf("%d", &x) != 1)\n            return 1;\n        if (i == 0 || x < best)\n            best = x;\n    }\n    printf("min = %d\\n", best);\n'),
        "positive": ("how many of the numbers are positive (greater than 0)", "positive", lambda v: f"positive = {sum(1 for x in v if x > 0)}",
                     '    int count = 0;\n    for (int i = 0; i < n; i++) {\n        int x;\n        if (scanf("%d", &x) != 1)\n            return 1;\n        if (x > 0)\n            count++;\n    }\n    printf("positive = %d\\n", count);\n'),
        "even": ("how many of the numbers are even", "even", lambda v: f"even = {sum(1 for x in v if x % 2 == 0)}",
                 '    int count = 0;\n    for (int i = 0; i < n; i++) {\n        int x;\n        if (scanf("%d", &x) != 1)\n            return 1;\n        if (x % 2 == 0)\n            count++;\n    }\n    printf("even = %d\\n", count);\n'),
        "average": ("the average of the numbers, with 2 decimals", "average", lambda v: f"average = {sum(v) / len(v):.2f}",
                    '    long total = 0;\n    for (int i = 0; i < n; i++) {\n        int x;\n        if (scanf("%d", &x) != 1)\n            return 1;\n        total += x;\n    }\n    printf("average = %.2f\\n", (double)total / n);\n'),
    }
    key = r.choice(list(kinds))
    what, label, spec, body = kinds[key]
    tests = []
    for k, size in enumerate([r.randint(3, 7), 1, r.randint(4, 8), r.randint(5, 9)]):
        lo = -40 if key in ("max", "min", "positive", "sum", "average") else 0
        v = [r.randint(lo, 99) for _ in range(size)]
        if k == 2 and size > 3:
            text = f"{size}\n" + " ".join(map(str, v[:3])) + "\n" + " ".join(map(str, v[3:])) + "\n"   # the numbers may span several lines
        else:
            text = f"{size}\n" + " ".join(map(str, v)) + "\n"
        tests.append({"stdin": text, "stdout": spec(v) + "\n"})
    return {
        "title": f"Numbers: {label}", "stars": 2, "summary": f"Read a list of integers and print {what}.",
        "statement": f"The first number on the input is `n` (at least 1), followed by `n` integers separated by spaces or line breaks. Print {what} in the form `{label} = <value>`.",
        "hints": ["Read `n` first with `scanf(\"%d\", &n)`, then loop `n` times reading one `int` each time.",
                  "You do not need to store the numbers: update your answer as each one is read."],
        "tests": tests,
        "solution": C('''#include <stdio.h>

int main(void)
{
    int n;

    if (scanf("%d", &n) != 1)
        return 1;
$body    return 0;
}
''', body=body),
    }


# ------------------------------------------------------------------ functions (a hidden main calls the student's function)


def functions_helper(r):
    names = ["convert", "score", "fee", "toll"]
    variants = []

    def v(name, args, ret, what, solution_body, py, tests):
        variants.append((name, args, ret, what, solution_body, py, tests))

    lo = r.randint(-20, 5)
    hi = lo + r.randint(10, 40)
    v("clamp", ["x", "lo", "hi"], "int", "`x` limited to the range `lo`..`hi`: `lo` if it is smaller, `hi` if it is larger, `x` otherwise",
      "    if (x < lo)\n        return lo;\n    if (x > hi)\n        return hi;\n    return x;\n",
      lambda x, lo, hi: min(max(x, lo), hi),
      [(r.randint(-60, 60), lo, hi) for _ in range(5)] + [(lo, lo, hi), (hi, lo, hi), (lo - 1, lo, hi)])
    v("gcd", ["a", "b"], "int", "the greatest common divisor of two positive integers",
      "    while (b != 0) {\n        int t = a % b;\n        a = b;\n        b = t;\n    }\n    return a;\n",
      math.gcd, [(r.randint(1, 90), r.randint(1, 90)) for _ in range(5)] + [(12, 12), (7, 1), (r.randint(2, 9) * 10, r.randint(2, 9) * 10)])
    v("power", ["base", "exponent"], "long", "`base` raised to the power `exponent` (both small and not negative; `power(b, 0)` is 1)",
      "    long result = 1;\n    for (int i = 0; i < exponent; i++)\n        result *= base;\n    return result;\n",
      lambda b, e: b ** e, [(r.randint(0, 9), r.randint(0, 9)) for _ in range(5)] + [(2, 10), (5, 0), (0, 3)])
    v("is_prime", ["n"], "int", "1 if `n` is a prime number and 0 if it is not (0 and 1 are not prime)",
      "    if (n < 2)\n        return 0;\n    for (int d = 2; d * d <= n; d++)\n        if (n % d == 0)\n            return 0;\n    return 1;\n",
      lambda n: int(n > 1 and all(n % d for d in range(2, int(n ** 0.5) + 1))),
      [(n,) for n in r.sample(range(0, 80), 7)] + [(2,), (1,)])
    a, b = r.randint(2, 9), r.randint(-9, 9)
    name = r.choice(names)
    v(name, ["x"], "int", f"`{a}` times `x` {'plus' if b >= 0 else 'minus'} `{abs(b)}`",
      f"    return {a} * x {'+' if b >= 0 else '-'} {abs(b)};\n",
      lambda x: a * x + b, [(r.randint(-20, 20),) for _ in range(5)] + [(0,), (1,)])
    name, args, ret, what, body, py, tests = r.choice(variants)
    proto = f"{ret} {name}({', '.join('int ' + x for x in args)})"
    n = len(args)
    fmt = "%ld" if ret == "long" else "%d"
    harness = C('''#include <stdio.h>

$proto;

int main(void)
{
    int $decl;

    while (scanf("$scan", $addr) == $n)
        printf("$name($shown) = $fmt\\n", $vars, $call);
    return 0;
}
''', proto=proto, decl=", ".join(args), scan=" ".join(["%d"] * n), addr=", ".join("&" + x for x in args), n=n, name=name,
              shown=", ".join(["%d"] * n), fmt=fmt, vars=", ".join(args), call=f"{name}({', '.join(args)})")
    cases = [{"stdin": " ".join(map(str, t)) + "\n", "stdout": f"{name}({', '.join(map(str, t))}) = {py(*t)}\n"} for t in tests]
    # one case with all the values at once (the main reads until the input ends)
    cases.append({"stdin": "".join(" ".join(map(str, t)) + "\n" for t in tests[:4]),
                  "stdout": "".join(f"{name}({', '.join(map(str, t))}) = {py(*t)}\n" for t in tests[:4])})
    example = cases[0]
    return {
        "title": f"Write `{name}`", "stars": 2 if n == 1 else 3, "summary": f"Implement the function `{name}`.",
        "statement": f"A hidden `main` reads the arguments from the input, calls your function and prints `{name}(" + ", ".join(args) + ") = <result>`. "
                     f"Write in `answer.c` (no `main`, no printing) the function that returns {what}:\n\n```c\n{proto};\n```",
        "hints": [f"The function only computes and returns: the hidden `main` does the reading and printing.", "Check the edge values the examples show: zero, equal arguments, the ends of a range."],
        "tests": cases, "harness": harness, "solution": f"{proto}\n{{\n{body}}}\n",
        "example": f"Input:\n```\n{example['stdin'].rstrip()}\n```\nOutput:\n```\n{example['stdout'].rstrip()}\n```\n",
    }


# ------------------------------------------------------------------ stdio (a file named on the command line)


def stdio_lines(r):
    def sentence():
        return " ".join(r.sample(WORDS, r.randint(2, 4)))

    kind = r.choice(["count", "long", "sum"])
    cases = []
    if kind == "count":
        word = r.choice(WORDS)
        what = f"the number of lines of FILE that contain the text `{word}` (anywhere in the line)"
        title, summary = "Lines containing a word", "Count the lines of a file that contain a text."
        for k in range(3):
            ls = [sentence() for _ in range(r.randint(3, 8))]
            for _ in range(r.randint(0, 2)):
                ls[r.randrange(len(ls))] += " " + word
            cases.append((ls, f"{sum(1 for x in ls if word in x)}\n"))
        cases.append(([], "0\n"))
        solution = C('''#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    char line[256];
    int count = 0;

    if (argc != 2)
        return 1;
    FILE *f = fopen(argv[1], "r");
    if (f == NULL)
        return 1;
    while (fgets(line, sizeof line, f) != NULL)
        if (strstr(line, "$word") != NULL)
            count++;
    fclose(f);
    printf("%d\\n", count);
    return 0;
}
''', word=word)
    elif kind == "long":
        limit = r.randint(14, 24)
        what = f"every line of FILE that is longer than {limit} characters (not counting the line break), preceded by its line number (the first line is 1) and `: `"
        title, summary = "Long lines", "Print the lines of a file that are longer than a limit, numbered."
        for k in range(3):
            ls = [sentence() for _ in range(r.randint(4, 8))]
            cases.append((ls, "".join(f"{i}: {x}\n" for i, x in enumerate(ls, 1) if len(x) > limit)))
        solution = C('''#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    char line[256];
    int number = 0;

    if (argc != 2)
        return 1;
    FILE *f = fopen(argv[1], "r");
    if (f == NULL)
        return 1;
    while (fgets(line, sizeof line, f) != NULL) {
        line[strcspn(line, "\\n")] = '\\0';
        number++;
        if (strlen(line) > $limit)
            printf("%d: %s\\n", number, line);
    }
    fclose(f);
    return 0;
}
''', limit=limit)
    else:
        what = "the sum of the integers in FILE (one per line, possibly negative)"
        title, summary = "Sum of a file", "Add the numbers stored one per line in a file."
        for k in range(3):
            v = [r.randint(-50, 99) for _ in range(r.randint(2, 8))]
            cases.append(([str(x) for x in v], f"{sum(v)}\n"))
        cases.append(([], "0\n"))
        solution = '''#include <stdio.h>

int main(int argc, char *argv[])
{
    int x;
    long total = 0;

    if (argc != 2)
        return 1;
    FILE *f = fopen(argv[1], "r");
    if (f == NULL)
        return 1;
    while (fscanf(f, "%d", &x) == 1)
        total += x;
    fclose(f);
    printf("%ld\\n", total);
    return 0;
}
'''
    tests = [{"cmd": '"$BIN" data.txt', "setup": lines_setup(ls), "stdout": out} for ls, out in cases]
    tests.append({"cmd": '"$BIN" missing.txt; echo "exit=$?"', "stdout": "exit=1\n"})
    shown, out = cases[0]
    example = "```\n$ cat data.txt\n" + "".join(x + "\n" for x in shown) + "$ ./prog data.txt\n" + out + "```\n"
    return {
        "title": title, "stars": 3, "summary": summary,
        "statement": f"`./prog FILE` prints {what}. If the program does not get exactly one argument, or the file cannot be opened, it prints nothing on standard output and exits with status 1.",
        "hints": ["Open the file with `fopen`, read it with `fgets` (or `fscanf`) in a loop until it returns `NULL` (or fails), and close it with `fclose`.",
                  "Check `argc` before using `argv[1]`, and check that `fopen` did not return `NULL`."],
        "tests": tests, "example": example, "solution": solution,
    }


# ------------------------------------------------------------------ build (a header, a source file and a hidden main)


def build_module(r):
    mod = r.choice(["calc", "numutil", "mathx", "arith"])
    variants = [
        ("gcd", "the greatest common divisor of two positive integers", "    while (b != 0) {\n        int t = a % b;\n        a = b;\n        b = t;\n    }\n    return a;\n", math.gcd, lambda: (r.randint(1, 80), r.randint(1, 80))),
        ("lcm", "the least common multiple of two positive integers (up to 99)", "    int x = a, y = b;\n\n    while (y != 0) {\n        int t = x % y;\n        x = y;\n        y = t;\n    }\n    return a / x * b;\n", lambda a, b: a * b // math.gcd(a, b), lambda: (r.randint(1, 30), r.randint(1, 30))),
        ("abs_diff", "the absolute value of the difference of two integers", "    return a > b ? a - b : b - a;\n", lambda a, b: abs(a - b), lambda: (r.randint(-50, 50), r.randint(-50, 50))),
        ("sum_squares", "the sum of the squares of two integers", "    return a * a + b * b;\n", lambda a, b: a * a + b * b, lambda: (r.randint(-30, 30), r.randint(-30, 30))),
    ]
    name, what, body, py, gen = r.choice(variants)
    guard = mod.upper() + "_H"
    header = f"#ifndef {guard}\n#define {guard}\n\nint {name}(int a, int b);\n\n#endif\n"
    main = C('''#include <stdio.h>
#include "$mod.h"

int main(void)
{
    int a, b;

    while (scanf("%d %d", &a, &b) == 2)
        printf("$name(%d, %d) = %d\\n", a, b, $name(a, b));
    return 0;
}
''', mod=mod, name=name)
    pairs = [gen() for _ in range(5)]
    cases = [{"stdin": f"{a} {b}\n", "stdout": f"{name}({a}, {b}) = {py(a, b)}\n"} for a, b in pairs]
    cases.append({"stdin": "".join(f"{a} {b}\n" for a, b in pairs[:3]), "stdout": "".join(f"{name}({a}, {b}) = {py(a, b)}\n" for a, b in pairs[:3])})
    return {
        "title": f"A module: {mod}", "stars": 3, "summary": f"Write the source file of a small module.",
        "statement": f"The header `{mod}.h` is given (it declares `int {name}(int a, int b);`) and a hidden `main.c` reads pairs of numbers and prints `{name}(a, b) = <result>`. "
                     f"Write `{mod}.c`: it must include `{mod}.h` and define `{name}`, which returns {what}.\n\nIt is built with `gcc -Wall -Wextra -o prog main.c {mod}.c`.",
        "hints": [f"Start `{mod}.c` with `#include \"{mod}.h\"` (quotes, because the header is in the same folder).", "The function only returns the value: `main.c` does the reading and printing."],
        "tests": cases, "files": [f"{mod}.c"], "build": f"gcc -Wall -Wextra -o prog main.c {mod}.c",
        "headers": {f"{mod}.h": header}, "hidden": {"main.c": main},
        "solution_files": {f"{mod}.c": f'#include "{mod}.h"\n\nint {name}(int a, int b)\n{{\n{body}}}\n'},
    }



# ------------------------------------------------------------------ variables, operators, conditions, loops


def variables_print(r):
    letter = r.choice("abcdefghijklmnopqrstuvwxy")
    if r.random() < 0.5:
        n, x = r.randint(-50, 400), round(r.uniform(1, 99), 3)
        title, summary = "Three variables", "Declare variables of three types and print them."
        statement = (f"Declare an `int` called `n` with the value `{n}`, a `double` called `x` with the value `{x}` and a `char` called `c` with the value `'{letter}'`. "
                     "Print them on one line as `n=<n> x=<x> c=<c>`, with `x` shown with 2 decimals.")
        out = f"n={n} x={x:.2f} c={letter}\n"
        body = f'    int n = {n};\n    double x = {x};\n    char c = \'{letter}\';\n\n    printf("n=%d x=%.2f c=%c\\n", n, x, c);\n'
        hints = ["`%d` prints an `int`, `%f` a `double` (`%.2f` rounds to 2 decimals) and `%c` a `char`.", "Declare each variable with its type and an initial value, e.g. `int n = 5;`."]
    else:
        title, summary = "A character and its code", "Print a char as a character and as a number."
        statement = (f"Declare a `char` called `c` with the value `'{letter}'`. Print one line `c=<c> code=<c as a number> next=<the next character of the alphabet>`.")
        out = f"c={letter} code={ord(letter)} next={chr(ord(letter) + 1)}\n"
        body = f'    char c = \'{letter}\';\n\n    printf("c=%c code=%d next=%c\\n", c, c, c + 1);\n'
        hints = ["A `char` is a small integer: `%c` prints it as a character and `%d` as its numeric code.", "The next character is `c + 1`."]
    return {"title": title, "stars": 1, "summary": summary, "statement": statement, "hints": hints, "tests": [{"args": [], "stdout": out}],
            "solution": "#include <stdio.h>\n\nint main(void)\n{\n" + body + "    return 0;\n}\n"}


def operators_arith(r):
    k, m = r.randint(2, 9), r.choice([7, 10, 11, 13, 16, 97])
    kinds = {
        "divmod": ("Quotient and remainder", "Integer division and remainder of two numbers.", "Print `<a> / <b> = <quotient>, <a> % <b> = <remainder>` (integer division).",
                   lambda a, b: f"{a} / {b} = {a // b}, {a} % {b} = {a % b}", lambda: (r.randint(0, 99), r.randint(1, 12)),
                   '        printf("%d / %d = %d, %d %% %d = %d\\n", a, b, a / b, a, b, a % b);\n'),
        "mix": ("A formula", "Evaluate an expression with several operators.", f"Print the value of `(a * {k} + b) % {m}`.",
                lambda a, b: str((a * k + b) % m), lambda: (r.randint(0, 99), r.randint(0, 99)),
                f'        printf("%d\\n", (a * {k} + b) % {m});\n'),
        "average": ("Average of two", "Average of two integers as a decimal number.", "Print their average with 2 decimals (the average of 3 and 4 is `3.50`).",
                    lambda a, b: f"{(a + b) / 2:.2f}", lambda: (r.randint(0, 99), r.randint(0, 99)),
                    '        printf("%.2f\\n", (a + b) / 2.0);\n'),
    }
    key = r.choice(list(kinds))
    title, summary, what, fn, gen, body = kinds[key]
    cases = []
    for size in (1, 1, 3):
        pairs = [gen() for _ in range(size)]
        cases.append({"stdin": "".join(f"{a} {b}\n" for a, b in pairs), "stdout": "".join(fn(a, b) + "\n" for a, b in pairs)})
    return {
        "title": title, "stars": 2, "summary": summary,
        "statement": f"The input has pairs of non-negative integers `a b` (`b` is never 0), one pair per line, until the input ends. For every pair print one line. {what}",
        "hints": ["`while (scanf(\"%d %d\", &a, &b) == 2)` repeats until the input ends: `scanf` returns how many values it read.",
                  "Remember that `/` between two `int`s is an integer division; write `2.0` or use a `double` when you need decimals; to print a `%` use `%%`."],
        "tests": cases,
        "solution": "#include <stdio.h>\n\nint main(void)\n{\n    int a, b;\n\n    while (scanf(\"%d %d\", &a, &b) == 2)\n" + body + "    return 0;\n}\n",
    }


def conditions_classify(r):
    if r.random() < 0.55:
        a, b = r.sample([2, 3, 4, 5, 6, 7], 2)
        w1, w2 = r.sample(["ping", "pong", "zip", "zap", "boom", "bang", "tick", "tock"], 2)
        title, summary = "Two words", "Say a word when a number is divisible by something."
        statement = (f"For every integer `n` of the input print one line: `{w1}` if `n` is divisible by {a}, `{w2}` if it is divisible by {b}, both words together (`{w1}{w2}`) if it is divisible by both, "
                     "and the number itself if by neither.")
        def say(n):
            t = (w1 if n % a == 0 else "") + (w2 if n % b == 0 else "")
            return t or str(n)
        gen = lambda: [r.randint(1, 60) for _ in range(r.randint(3, 8))]  # noqa: E731
        cases = []
        for k in range(4):
            v = gen()
            if k == 0:
                v = [a * b, a, b, a * b + 1]
            cases.append({"stdin": " ".join(map(str, v)) + "\n", "stdout": "".join(say(n) + "\n" for n in v)})
        body = (f"    int n;\n\n    while (scanf(\"%d\", &n) == 1) {{\n        int said = 0;\n\n        if (n % {a} == 0) {{\n            printf(\"{w1}\");\n            said = 1;\n        }}\n"
                f"        if (n % {b} == 0) {{\n            printf(\"{w2}\");\n            said = 1;\n        }}\n        if (!said)\n            printf(\"%d\", n);\n        printf(\"\\n\");\n    }}\n")
        hints = ["`n % k == 0` tells whether `n` is divisible by `k`.", "Test both divisors independently: one `if` for each, and remember whether you printed a word."]
        stars = 2
    else:
        t1 = r.randint(80, 95)
        t2 = t1 - r.randint(8, 15)
        t3 = t2 - r.randint(8, 15)
        title, summary = "Grades", "Turn scores into letters."
        statement = f"For every score (an integer from 0 to 100) of the input print one line with its grade: `A` for {t1} or more, `B` for {t2} or more, `C` for {t3} or more and `F` below that."
        def grade(s):
            return "A" if s >= t1 else "B" if s >= t2 else "C" if s >= t3 else "F"
        cases = []
        for k in range(4):
            v = [r.randint(0, 100) for _ in range(r.randint(3, 7))]
            if k == 0:
                v = [t1, t1 - 1, t2, t2 - 1, t3, t3 - 1, 100, 0]
            cases.append({"stdin": " ".join(map(str, v)) + "\n", "stdout": "".join(grade(n) + "\n" for n in v)})
        body = (f"    int score;\n\n    while (scanf(\"%d\", &score) == 1) {{\n        if (score >= {t1})\n            printf(\"A\\n\");\n        else if (score >= {t2})\n            printf(\"B\\n\");\n"
                f"        else if (score >= {t3})\n            printf(\"C\\n\");\n        else\n            printf(\"F\\n\");\n    }}\n")
        hints = ["`while (scanf(\"%d\", &score) == 1)` reads numbers until the input ends.", "Test the highest threshold first and use `else if` for the others."]
        stars = 2
    return {"title": title, "stars": stars, "summary": summary, "statement": statement, "hints": hints, "tests": cases,
            "solution": "#include <stdio.h>\n\nint main(void)\n{\n" + body + "    return 0;\n}\n"}


def loops_series(r):
    kind = r.choice(["multiples", "squares", "triangle"])
    if kind == "multiples":
        k = r.randint(2, 9)
        title, summary = f"Multiples of {k}", "Print a series with a loop."
        statement = f"Read an integer `n` (at least {k}) and print the multiples of {k} that are not greater than `n`, on one line, in increasing order, separated by single spaces."
        out = lambda n: " ".join(str(i) for i in range(k, n + 1, k)) + "\n"  # noqa: E731
        ns = [k, k * 3 + 1, r.randint(k, 60), r.randint(k, 60)]
        body = f"    for (int i = {k}; i <= n; i += {k}) {{\n        if (i != {k})\n            printf(\" \");\n        printf(\"%d\", i);\n    }}\n    printf(\"\\n\");\n"
        hints = [f"A `for` loop can step by {k}: `i += {k}`.", "Print the separator before every number except the first one, so the line has no trailing space."]
    elif kind == "squares":
        title, summary = "Sum of squares", "Add up a series with a loop."
        statement = "Read an integer `n` (at least 1) and print `sum = <S>`, where S is 1\u00b2 + 2\u00b2 + \u2026 + n\u00b2."
        out = lambda n: f"sum = {sum(i * i for i in range(1, n + 1))}\n"  # noqa: E731
        ns = [1, 2, r.randint(3, 20), r.randint(20, 100)]
        body = "    long sum = 0;\n\n    for (int i = 1; i <= n; i++)\n        sum += (long)i * i;\n    printf(\"sum = %ld\\n\", sum);\n"
        hints = ["Keep a running total that starts at 0 and add `i * i` on each turn of the loop.", "Use a `long` for the total: the sum grows quickly."]
    else:
        ch = r.choice("*#@+")
        title, summary = "A triangle", "Print a figure with two nested loops."
        statement = f"Read an integer `n` (at least 1) and print a triangle of `{ch}` with `n` rows: row 1 has one `{ch}`, row 2 has two, and so on."
        out = lambda n: "".join(ch * i + "\n" for i in range(1, n + 1))  # noqa: E731
        ns = [1, 3, r.randint(4, 8), r.randint(5, 10)]
        body = f"    for (int row = 1; row <= n; row++) {{\n        for (int i = 0; i < row; i++)\n            putchar('{ch}');\n        putchar('\\n');\n    }}\n"
        hints = ["An outer loop for the rows and an inner loop that prints the characters of that row.", "Row number `row` has `row` characters: end it with a newline."]
    return {"title": title, "stars": 2, "summary": summary, "statement": statement, "hints": hints,
            "tests": [{"stdin": f"{n}\n", "stdout": out(n)} for n in ns],
            "solution": "#include <stdio.h>\n\nint main(void)\n{\n    int n;\n\n    if (scanf(\"%d\", &n) != 1)\n        return 1;\n" + body + "    return 0;\n}\n"}


# ------------------------------------------------------------------ arrays, strings


def arrays_ops(r):
    kind = r.choice(["reverse", "rotate", "above"])
    k = r.randint(1, 3)

    def lists():
        out = []
        for size in (r.randint(4, 8), r.randint(4, 12), k + 2):
            v = [r.randint(-30, 60) for _ in range(size)]
            if len(set(v)) == 1:
                v[0] += 1
            out.append(v)
        return out

    if kind == "reverse":
        title, summary = "Reversed numbers", "Store numbers in an array and print them backwards."
        statement = "Print the numbers in reverse order, on one line, separated by single spaces."
        fn = lambda v: v[::-1]  # noqa: E731
        body = "    for (int i = n - 1; i >= 0; i--) {\n        printf(\"%d\", a[i]);\n        printf(i > 0 ? \" \" : \"\\n\");\n    }\n"
        hints = ["Store the numbers in an array while reading them, then loop from `n - 1` down to 0.", "Print a space after every number except the last one."]
    elif kind == "rotate":
        title, summary = f"Rotate left by {k}", "Print an array starting from another position."
        statement = f"Print the numbers on one line, separated by single spaces, rotated {k} position{'s' if k > 1 else ''} to the left: the first {k} number{'s' if k > 1 else ''} go to the end. (`n` is always greater than {k}.)"
        fn = lambda v: v[k:] + v[:k]  # noqa: E731
        body = f"    for (int i = 0; i < n; i++) {{\n        printf(\"%d\", a[(i + {k}) % n]);\n        printf(i < n - 1 ? \" \" : \"\\n\");\n    }}\n"
        hints = [f"The element printed at step `i` is `a[(i + {k}) % n]`.", "The `%` operator wraps the index around to the start."]
    else:
        title, summary = "Above the average", "Compare every element with the average."
        statement = "Print, on one line separated by single spaces and in input order, the numbers that are greater than the average of all the numbers. (They are never all equal, so there is always at least one.)"
        fn = lambda v: [x for x in v if x * len(v) > sum(v)]  # noqa: E731
        body = ("    long sum = 0;\n    int first = 1;\n\n    for (int i = 0; i < n; i++)\n        sum += a[i];\n    for (int i = 0; i < n; i++) {\n        if ((long)a[i] * n > sum) {\n"
                "            if (!first)\n                printf(\" \");\n            printf(\"%d\", a[i]);\n            first = 0;\n        }\n    }\n    printf(\"\\n\");\n")
        hints = ["First pass: add the numbers. Second pass: print those greater than the average.", "To compare without decimals, test `a[i] * n > sum` instead of dividing."]
    cases = [{"stdin": f"{len(v)}\n" + " ".join(map(str, v)) + "\n", "stdout": " ".join(map(str, fn(v))) + "\n"} for v in lists()]
    return {
        "title": title, "stars": 3, "summary": summary,
        "statement": "The first number on the input is `n` (from 2 to 20), followed by `n` integers. Store them in an array. " + statement,
        "hints": hints, "tests": cases,
        "solution": "#include <stdio.h>\n\nint main(void)\n{\n    int a[20];\n    int n;\n\n    if (scanf(\"%d\", &n) != 1)\n        return 1;\n    for (int i = 0; i < n; i++)\n        if (scanf(\"%d\", &a[i]) != 1)\n            return 1;\n" + body + "    return 0;\n}\n",
    }


def strings_ops(r):
    def sentence():
        return " ".join(r.sample(WORDS, r.randint(2, 3)))

    kind = r.choice(["reverse", "replace", "vowels", "capitalize", "novowels"])
    x, y = r.choice("aeo"), r.choice("xyzq")
    if kind == "reverse":
        title, summary = "Reversed line", "Print a string backwards."
        statement = "Print the line reversed."
        fn = lambda s: s[::-1] + "\n"  # noqa: E731
        body = "    for (int i = (int)strlen(s) - 1; i >= 0; i--)\n        putchar(s[i]);\n    putchar('\\n');\n"
        hints = ["`strlen(s)` is the number of characters; the last one is `s[strlen(s) - 1]`.", "`fgets` keeps the line break at the end: remove it first (for instance with `s[strcspn(s, \"\\n\")] = '\\0';`)."]
    elif kind == "replace":
        title, summary = f"Replace {x} by {y}", "Change characters of a string."
        statement = f"Print the line with every `{x}` replaced by `{y}`."
        fn = lambda s: s.replace(x, y) + "\n"  # noqa: E731
        body = f"    for (int i = 0; s[i] != '\\0'; i++)\n        if (s[i] == '{x}')\n            s[i] = '{y}';\n    puts(s);\n"
        hints = ["A string is an array of `char`: you can change `s[i]` in place.", "Stop at the terminating `'\\0'`."]
    elif kind == "vowels":
        title, summary = "Count the vowels", "Count characters of a string."
        statement = "Print `vowels = <n>`, where n is the number of vowels (`a e i o u`, lowercase) in the line."
        fn = lambda s: f"vowels = {sum(c in 'aeiou' for c in s)}\n"  # noqa: E731
        body = "    int count = 0;\n\n    for (int i = 0; s[i] != '\\0'; i++)\n        if (strchr(\"aeiou\", s[i]) != NULL)\n            count++;\n    printf(\"vowels = %d\\n\", count);\n"
        hints = ["Go through the string until `'\\0'` and count the characters that are vowels.", "`strchr(\"aeiou\", c)` is not `NULL` when `c` is one of them."]
    elif kind == "capitalize":
        title, summary = "Capitalise the words", "Change the case of letters."
        statement = "The words of the line are separated by single spaces and are in lowercase. Print the line with the first letter of every word in uppercase."
        fn = lambda s: " ".join(w.capitalize() for w in s.split(" ")) + "\n"  # noqa: E731
        body = "    int start = 1;\n\n    for (int i = 0; s[i] != '\\0'; i++) {\n        if (start)\n            s[i] = (char)toupper((unsigned char)s[i]);\n        start = (s[i] == ' ');\n    }\n    puts(s);\n"
        hints = ["A letter starts a word when it is the first one or the one before it is a space.", "`toupper` (from `<ctype.h>`) converts a letter to uppercase."]
    else:
        title, summary = "Without vowels", "Build a filtered copy of a string."
        statement = "Print the line without its vowels (`a e i o u`; the rest, including spaces, stays the same)."
        fn = lambda s: "".join(c for c in s if c not in "aeiou") + "\n"  # noqa: E731
        body = "    for (int i = 0; s[i] != '\\0'; i++)\n        if (strchr(\"aeiou\", s[i]) == NULL)\n            putchar(s[i]);\n    putchar('\\n');\n"
        hints = ["Print only the characters that are not vowels.", "`strchr(\"aeiou\", c)` is `NULL` when `c` is not a vowel."]
    lines = []
    while len(lines) < 4:
        s = sentence()
        if kind == "replace" and x not in s:
            continue
        lines.append(s)
    includes = "#include <ctype.h>\n#include <stdio.h>\n#include <string.h>\n" if kind == "capitalize" else "#include <stdio.h>\n#include <string.h>\n"
    return {
        "title": title, "stars": 3, "summary": summary,
        "statement": "Read one line of text (at most 80 characters) from the input. " + statement,
        "hints": hints, "tests": [{"stdin": s + "\n", "stdout": fn(s)} for s in lines],
        "solution": includes + "\nint main(void)\n{\n    char s[100];\n\n    if (fgets(s, sizeof s, stdin) == NULL)\n        return 1;\n    s[strcspn(s, \"\\n\")] = '\\0';\n" + body + "    return 0;\n}\n",
    }


# ------------------------------------------------------------------ pointers, memory, structs, arguments


def pointers_helper(r):
    kind = r.choice(["divmod", "minmax", "time", "order"])
    if kind == "divmod":
        proto = "void divmod(int a, int b, int *q, int *r)"
        what = "stores the quotient of `a / b` in `*q` and the remainder in `*r` (`a >= 0`, `b > 0`)"
        body = "    *q = a / b;\n    *r = a % b;\n"
        harness = ('#include <stdio.h>\n\nvoid divmod(int a, int b, int *q, int *r);\n\nint main(void)\n{\n    int a, b, q, r;\n\n    while (scanf("%d %d", &a, &b) == 2) {\n        q = r = -1;\n'
                   '        divmod(a, b, &q, &r);\n        printf("divmod(%d, %d): q=%d r=%d\\n", a, b, q, r);\n    }\n    return 0;\n}\n')
        pairs = [(r.randint(0, 99), r.randint(1, 12)) for _ in range(5)] + [(0, 5), (7, 7)]
        rows = [(f"{a} {b}\n", f"divmod({a}, {b}): q={a // b} r={a % b}\n") for a, b in pairs]
        title, example_in = "Two results", None
    elif kind == "minmax":
        proto = "void minmax(const int *v, int n, int *min, int *max)"
        what = "stores the smallest of the `n` numbers of `v` in `*min` and the largest in `*max` (`n >= 1`)"
        body = "    *min = *max = v[0];\n    for (int i = 1; i < n; i++) {\n        if (v[i] < *min)\n            *min = v[i];\n        if (v[i] > *max)\n            *max = v[i];\n    }\n"
        harness = ('#include <stdio.h>\n\nvoid minmax(const int *v, int n, int *min, int *max);\n\nint main(void)\n{\n    int v[20];\n    int n, lo, hi;\n\n    while (scanf("%d", &n) == 1) {\n        for (int i = 0; i < n; i++)\n'
                   '            if (scanf("%d", &v[i]) != 1)\n                return 1;\n        minmax(v, n, &lo, &hi);\n        printf("min=%d max=%d\\n", lo, hi);\n    }\n    return 0;\n}\n')
        rows = []
        for size in (1, 2, r.randint(3, 8), r.randint(3, 12), r.randint(5, 15)):
            v = [r.randint(-40, 80) for _ in range(size)]
            rows.append((f"{size}\n" + " ".join(map(str, v)) + "\n", f"min={min(v)} max={max(v)}\n"))
        title, example_in = "Minimum and maximum", None
    elif kind == "time":
        proto = "void split_time(int total, int *h, int *m, int *s)"
        what = "converts `total` seconds into hours, minutes and seconds (`*h`, `*m`, `*s`, with `*m` and `*s` below 60)"
        body = "    *h = total / 3600;\n    *m = total % 3600 / 60;\n    *s = total % 60;\n"
        harness = ('#include <stdio.h>\n\nvoid split_time(int total, int *h, int *m, int *s);\n\nint main(void)\n{\n    int total, h, m, s;\n\n    while (scanf("%d", &total) == 1) {\n'
                   '        h = m = s = -1;\n        split_time(total, &h, &m, &s);\n        printf("%d s = %d:%02d:%02d\\n", total, h, m, s);\n    }\n    return 0;\n}\n')
        rows = [(f"{t}\n", f"{t} s = {t // 3600}:{t % 3600 // 60:02d}:{t % 60:02d}\n") for t in [r.randint(0, 86399) for _ in range(4)] + [0, 59, 3600, 3661]]
        title, example_in = "Hours, minutes, seconds", None
    else:
        proto = "void order(int *a, int *b)"
        what = "swaps the two values if needed so that afterwards `*a <= *b`"
        body = "    if (*a > *b) {\n        int t = *a;\n\n        *a = *b;\n        *b = t;\n    }\n"
        harness = ('#include <stdio.h>\n\nvoid order(int *a, int *b);\n\nint main(void)\n{\n    int a, b;\n\n    while (scanf("%d %d", &a, &b) == 2) {\n        order(&a, &b);\n        printf("%d %d\\n", a, b);\n    }\n    return 0;\n}\n')
        pairs = [(r.randint(-30, 30), r.randint(-30, 30)) for _ in range(5)] + [(5, 5), (9, -9)]
        rows = [(f"{a} {b}\n", f"{min(a, b)} {max(a, b)}\n") for a, b in pairs]
        title, example_in = "Put in order", None
    cases = [{"stdin": i, "stdout": o} for i, o in rows]
    cases.append({"stdin": "".join(i for i, _ in rows[:3]), "stdout": "".join(o for _, o in rows[:3])})
    return {
        "title": title, "stars": 3, "summary": "Return results through pointers.",
        "statement": f"A hidden `main` calls your function and prints what it finds in the variables whose addresses it passes. Write in `answer.c` (no `main`, no printing):\n\n```c\n{proto};\n```\n\nThe function {what}.",
        "hints": ["A pointer parameter lets the function change a variable of the caller: write through it with `*p = value;`.", "Do not print anything: the hidden `main` prints the variables after the call."],
        "tests": cases, "harness": harness, "solution": f"{proto}\n{{\n{body}}}\n",
    }


def memory_dynamic(r):
    kind = r.choice(["reverse", "sorted"])
    if kind == "reverse":
        title, what = "Backwards from the heap", "in reverse order"
        body = "    for (int i = n - 1; i >= 0; i--)\n        printf(\"%d%c\", a[i], i > 0 ? ' ' : '\\n');\n"
        fn = lambda v: v[::-1]  # noqa: E731
        pre = ""
        hints = ["`malloc(n * sizeof *a)` gives room for `n` integers; check that it did not return `NULL`.", "Release the memory with `free` before the program ends."]
    else:
        title, what = "Sorted from the heap", "in increasing order"
        body = "    qsort(a, n, sizeof *a, cmp);\n    for (int i = 0; i < n; i++)\n        printf(\"%d%c\", a[i], i < n - 1 ? ' ' : '\\n');\n"
        fn = sorted
        pre = "static int cmp(const void *x, const void *y)\n{\n    int p = *(const int *)x, q = *(const int *)y;\n\n    return (p > q) - (p < q);\n}\n\n"
        hints = ["`qsort(base, count, size, compare)` sorts an array in place; the comparison function receives two `const void *` pointing at elements.", "The comparison returns negative, zero or positive: `(p > q) - (p < q)` does that without overflow."]
    cases = []
    for size in (1, r.randint(2, 6), r.randint(5, 12), r.randint(8, 15)):
        v = [r.randint(-30, 90) for _ in range(size)]
        cases.append({"stdin": f"{size}\n" + " ".join(map(str, v)) + "\n", "stdout": " ".join(map(str, fn(v))) + "\n"})
    return {
        "title": title, "stars": 3, "summary": "Keep numbers in memory obtained with malloc.",
        "statement": f"The first number on the input is `n` (at least 1, no upper limit that you may assume), followed by `n` integers. Keep them in an array obtained with `malloc` and print them {what} on one line, separated by single spaces. Free the memory at the end.",
        "hints": hints, "tests": cases,
        "solution": "#include <stdio.h>\n#include <stdlib.h>\n\n" + pre + "int main(void)\n{\n    int n;\n    int *a;\n\n    if (scanf(\"%d\", &n) != 1)\n        return 1;\n    a = malloc(n * sizeof *a);\n    if (a == NULL)\n        return 1;\n    for (int i = 0; i < n; i++)\n        if (scanf(\"%d\", &a[i]) != 1)\n            return 1;\n" + body + "    free(a);\n    return 0;\n}\n",
    }


def structs_records(r):
    sname = r.choice(["item", "product", "part", "entry"])
    kind = r.choice(["total", "best"])
    cases = []
    for size in (1, r.randint(2, 5), r.randint(4, 8), r.randint(3, 8)):
        names = r.sample(WORDS, size)
        recs = [(nm, r.randint(1, 9), r.randint(1, 50)) for nm in names]
        if kind == "total":
            out = f"total = {sum(q * p for _, q, p in recs)}\n"
        else:
            best = recs[0]
            for rec in recs[1:]:
                if rec[1] * rec[2] > best[1] * best[2]:
                    best = rec
            out = f"best = {best[0]}\n"
        cases.append({"stdin": f"{size}\n" + "".join(f"{nm} {q} {p}\n" for nm, q, p in recs), "stdout": out})
    if kind == "total":
        what, title = "the total value of all the records, as `total = <value>`", "Total value"
        body = "    long total = 0;\n\n    for (int i = 0; i < n; i++)\n        total += (long)list[i].qty * list[i].price;\n    printf(\"total = %ld\\n\", total);\n"
    else:
        what, title = "the name of the record with the largest value, as `best = <name>` (the first one if several tie)", "Most valuable"
        body = "    int best = 0;\n\n    for (int i = 1; i < n; i++)\n        if (list[i].qty * list[i].price > list[best].qty * list[best].price)\n            best = i;\n    printf(\"best = %s\\n\", list[best].name);\n"
    return {
        "title": title, "stars": 3, "summary": "Read records into an array of structs.",
        "statement": f"The first number on the input is `n` (from 1 to 20), followed by `n` records, one per line: a `name` (one word of at most 31 characters), a `qty` and a `price` (integers). The value of a record is `qty * price`. Keep the records in an array of a `struct {sname}` and print {what}.",
        "hints": [f"Define `struct {sname} {{ char name[32]; int qty; int price; }};` and an array of them.", "`scanf(\"%31s %d %d\", rec.name, &rec.qty, &rec.price)` reads a record (the array `name` is already an address: no `&`)."],
        "tests": cases,
        "solution": f"#include <stdio.h>\n\nstruct {sname} {{\n    char name[32];\n    int qty;\n    int price;\n}};\n\nint main(void)\n{{\n    struct {sname} list[20];\n    int n;\n\n    if (scanf(\"%d\", &n) != 1)\n        return 1;\n    for (int i = 0; i < n; i++)\n        if (scanf(\"%31s %d %d\", list[i].name, &list[i].qty, &list[i].price) != 3)\n            return 1;\n" + body + "    return 0;\n}\n",
    }


def args_tool(r):
    kind = r.choice(["reverse", "sum", "longest", "count"])

    def words(k):
        out = r.sample(WORDS, k)
        if r.random() < 0.5:
            out[r.randrange(k)] = " ".join(r.sample(WORDS, 2))   # one argument with a space in it
        return out

    cases = []
    if kind == "reverse":
        title, what = "Arguments backwards", "Print the arguments in reverse order, one per line. With no arguments print nothing."
        for k in (0, 1, r.randint(2, 4), r.randint(3, 5)):
            a = words(k) if k else []
            cases.append({"args": a, "stdout": "".join(x + "\n" for x in reversed(a))})
        body = "    for (int i = argc - 1; i >= 1; i--)\n        printf(\"%s\\n\", argv[i]);\n"
        hints = ["`argv[0]` is the program name; the arguments are `argv[1]` to `argv[argc - 1]`.", "Loop with the index going down from `argc - 1` to 1."]
    elif kind == "sum":
        title, what = "Sum of the arguments", "Every argument is an integer (possibly negative). Print `sum = <total>`; with no arguments the sum is 0."
        for k in (0, 1, r.randint(2, 4), r.randint(3, 6)):
            a = [str(r.randint(-50, 99)) for _ in range(k)]
            cases.append({"args": a, "stdout": f"sum = {sum(map(int, a))}\n"})
        body = "    long sum = 0;\n\n    for (int i = 1; i < argc; i++)\n        sum += atoi(argv[i]);\n    printf(\"sum = %ld\\n\", sum);\n"
        hints = ["Arguments are strings: `atoi` (from `<stdlib.h>`) converts one to an `int`.", "Start the loop at 1: `argv[0]` is the program name."]
    elif kind == "longest":
        title, what = "Longest argument", "Print the longest argument (the first one if several have the same length). With no arguments print nothing and exit with status 1."
        for k in (0, 1, r.randint(2, 4), r.randint(3, 5)):
            a = words(k) if k else []
            best = max(a, key=len) if a else ""
            if a:
                best = a[0]
                for x in a[1:]:
                    if len(x) > len(best):
                        best = x
            cases.append({"args": a, "stdout": best + "\n" if a else "", "exit": 0 if a else 1})
        body = "    int best = 1;\n\n    if (argc < 2)\n        return 1;\n    for (int i = 2; i < argc; i++)\n        if (strlen(argv[i]) > strlen(argv[best]))\n            best = i;\n    printf(\"%s\\n\", argv[best]);\n"
        hints = ["`strlen` (from `<string.h>`) gives the length of a string.", "Keep the index of the best one so far and replace it only when another is strictly longer."]
    else:
        title, what = "Numbered arguments", "Print `<n> argument(s)` first (it is `1 argument` for one, `0 arguments` for none), then one line `<i>: <argument>` for each, numbering from 1."
        for k in (0, 1, r.randint(2, 4), r.randint(3, 5)):
            a = words(k) if k else []
            cases.append({"args": a, "stdout": f"{k} argument{'' if k == 1 else 's'}\n" + "".join(f"{i}: {x}\n" for i, x in enumerate(a, 1))})
        body = "    printf(\"%d argument%s\\n\", argc - 1, argc == 2 ? \"\" : \"s\");\n    for (int i = 1; i < argc; i++)\n        printf(\"%d: %s\\n\", i, argv[i]);\n"
        hints = ["`argc - 1` is the number of arguments, because `argv[0]` is the program name.", "Only the singular is special: `argc == 2` means exactly one argument."]
    inc = {"sum": "#include <stdio.h>\n#include <stdlib.h>\n", "longest": "#include <stdio.h>\n#include <string.h>\n"}.get(kind, "#include <stdio.h>\n")
    return {
        "title": title, "stars": 2, "summary": "Work with the command-line arguments.",
        "statement": what + " An argument may contain spaces (`./prog \"two words\"` is one argument).",
        "hints": hints, "tests": cases,
        "solution": inc + "\nint main(int argc, char *argv[])\n{\n" + body + "    return 0;\n}\n",
    }


# ------------------------------------------------------------------ system calls, directories, errors, whole programs


def syscalls_file(r):
    kind = r.choice(["count", "size", "copy"])
    ch = r.choice("aeiou")

    def text():
        return [" ".join(r.sample(WORDS, r.randint(2, 4))) for _ in range(r.randint(2, 9))]

    cases = []
    if kind == "count":
        title, what = f"Count the `{ch}` bytes", f"prints how many bytes of FILE are equal to `{ch}`"
        for _ in range(3):
            ls = text()
            cases.append((f'"$BIN" data.txt', ls, f"{sum(l.count(ch) for l in ls)}\n"))
        body = f"    char buf[64];\n    ssize_t got;\n    long count = 0;\n\n    if (argc != 2)\n        return 1;\n    int fd = open(argv[1], O_RDONLY);\n    if (fd < 0)\n        return 1;\n    while ((got = read(fd, buf, sizeof buf)) > 0)\n        for (ssize_t i = 0; i < got; i++)\n            if (buf[i] == '{ch}')\n                count++;\n    close(fd);\n    printf(\"%ld\\n\", count);\n"
        use = "`open` (`O_RDONLY`), `read` into a small buffer in a loop until it returns 0, and `close`"
        hints = ["`read(fd, buf, n)` returns how many bytes it really read (0 at the end, -1 on error): process exactly that many.", "The file may be longer than your buffer: loop until `read` returns 0."]
    elif kind == "size":
        title, what = "Size of a file", "prints the size of FILE in bytes"
        for _ in range(3):
            ls = text()
            cases.append((f'"$BIN" data.txt', ls, f"{sum(len(l) + 1 for l in ls)}\n"))
        body = "    if (argc != 2)\n        return 1;\n    int fd = open(argv[1], O_RDONLY);\n    if (fd < 0)\n        return 1;\n    off_t size = lseek(fd, 0, SEEK_END);\n    close(fd);\n    printf(\"%ld\\n\", (long)size);\n"
        use = "`open`, `lseek` and `close`, without reading the contents"
        hints = ["`lseek(fd, 0, SEEK_END)` moves to the end of the file and returns the new position, which is the size.", "Do not read the file: only ask for its position."]
    else:
        title, what = "Copy a file", "copies FILE to DEST (creating it, or emptying it if it exists) and prints nothing"
        for _ in range(3):
            ls = text()
            cases.append(('"$BIN" data.txt copy.txt && cat copy.txt', ls, "".join(l + "\n" for l in ls)))
        body = "    char buf[64];\n    ssize_t got;\n\n    if (argc != 3)\n        return 1;\n    int in = open(argv[1], O_RDONLY);\n    if (in < 0)\n        return 1;\n    int out = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);\n    if (out < 0)\n        return 1;\n    while ((got = read(in, buf, sizeof buf)) > 0)\n        if (write(out, buf, got) != got)\n            return 1;\n    close(in);\n    close(out);\n"
        use = "`open` (with `O_WRONLY | O_CREAT | O_TRUNC` and a mode such as `0644` for the new file), `read`, `write` and `close`"
        hints = ["Read a block, write exactly the bytes you read, and repeat until `read` returns 0.", "The destination needs `O_CREAT` and a mode (`0644`), and `O_TRUNC` to empty an existing file."]
    tests = [{"cmd": cmd, "setup": lines_setup(ls), "stdout": out} for cmd, ls, out in cases]
    miss = '"$BIN" missing.txt copy.txt; echo "exit=$?"' if kind == "copy" else '"$BIN" missing.txt; echo "exit=$?"'
    tests.append({"cmd": miss, "stdout": "exit=1\n"})
    shown = cases[0]
    example = "```\n$ cat data.txt\n" + "".join(l + "\n" for l in shown[1]) + "$ ./prog data.txt" + (" copy.txt && cat copy.txt" if kind == "copy" else "") + "\n" + shown[2] + "```\n"
    usage = "`./prog SRC DEST`" if kind == "copy" else "`./prog FILE`"
    return {
        "title": title, "stars": 3, "summary": "Use the system calls of files directly.",
        "statement": f"{usage} {what}. Use the system calls {use}, not the `stdio` functions for the file. If an argument is missing or a file cannot be opened, print nothing and exit with status 1.",
        "hints": hints, "tests": tests, "example": example,
        "solution": "#include <fcntl.h>\n#include <stdio.h>\n#include <unistd.h>\n\nint main(int argc, char *argv[])\n{\n" + body + "    return 0;\n}\n",
    }


def directories_list(r):
    kind = r.choice(["count", "ext", "sorted"])
    ext = r.choice(["txt", "log", "dat", "cfg"])

    def names(k, e):
        pool = r.sample(WORDS, k)
        return [f"{n}.{r.choice([e, e, 'tmp', 'bak'])}" for n in pool]

    setups, outs, listings = [], [], []
    for _ in range(3):
        files = names(r.randint(2, 7), ext)
        subs = r.sample([w for w in WORDS if all(not f.startswith(w + ".") for f in files)], r.randint(0, 2))
        setup = "mkdir -p d && cd d && touch " + " ".join(files) + (" && mkdir " + " ".join(subs) if subs else "")
        setups.append(setup)
        listings.append(sorted(files + subs))
        if kind == "count":
            outs.append(f"{len(files) + len(subs)}\n")
        elif kind == "ext":
            outs.append("".join(sorted(f + "\n" for f in files if f.endswith("." + ext))))
        else:
            outs.append("".join(x + "\n" for x in sorted(files + subs)))
    if kind == "count":
        title, what = "Count the entries", "prints the number of entries in the folder DIR (files and folders, not `.` and `..`)"
        body = ("    DIR *d;\n    struct dirent *e;\n    int count = 0;\n\n    if (argc != 2)\n        return 1;\n    d = opendir(argv[1]);\n    if (d == NULL)\n        return 1;\n    while ((e = readdir(d)) != NULL)\n"
                "        if (strcmp(e->d_name, \".\") != 0 && strcmp(e->d_name, \"..\") != 0)\n            count++;\n    closedir(d);\n    printf(\"%d\\n\", count);\n")
        hints = ["`opendir` returns a `DIR *` (or `NULL`); `readdir` returns the next entry until it returns `NULL`.", "`.` and `..` are entries too: skip them with `strcmp` on `e->d_name`."]
        inc = "#include <dirent.h>\n#include <stdio.h>\n#include <string.h>\n"
        match = "exact"
    elif kind == "ext":
        title, what = f"Files ending in .{ext}", f"prints the names of the entries of DIR that end with `.{ext}`, one per line, in any order"
        body = (f"    DIR *d;\n    struct dirent *e;\n\n    if (argc != 2)\n        return 1;\n    d = opendir(argv[1]);\n    if (d == NULL)\n        return 1;\n    while ((e = readdir(d)) != NULL) {{\n        char *dot = strrchr(e->d_name, '.');\n\n"
                f"        if (dot != NULL && strcmp(dot, \".{ext}\") == 0)\n            printf(\"%s\\n\", e->d_name);\n    }}\n    closedir(d);\n")
        hints = ["`strrchr(name, '.')` finds the last dot of a name (or returns `NULL`).", f"Compare what follows the dot with `strcmp(dot, \".{ext}\")`."]
        inc = "#include <dirent.h>\n#include <stdio.h>\n#include <string.h>\n"
        match = "sorted"
    else:
        title, what = "Sorted listing", "prints the names of the entries of DIR (not `.` and `..`), one per line, in alphabetical order"
        body = ("    DIR *d;\n    struct dirent *e;\n    char names[64][256];\n    int n = 0;\n\n    if (argc != 2)\n        return 1;\n    d = opendir(argv[1]);\n    if (d == NULL)\n        return 1;\n    while ((e = readdir(d)) != NULL)\n"
                "        if (strcmp(e->d_name, \".\") != 0 && strcmp(e->d_name, \"..\") != 0 && n < 64)\n            strcpy(names[n++], e->d_name);\n    closedir(d);\n    qsort(names, n, sizeof names[0], cmp);\n    for (int i = 0; i < n; i++)\n        printf(\"%s\\n\", names[i]);\n")
        hints = ["`readdir` gives the entries in no particular order: collect the names in an array first, then sort it.", "`qsort` with a comparison function that calls `strcmp` sorts an array of strings."]
        inc = "#include <dirent.h>\n#include <stdio.h>\n#include <stdlib.h>\n#include <string.h>\n\nstatic int cmp(const void *a, const void *b)\n{\n    return strcmp(a, b);\n}\n"
        match = "exact"
    tests = [{"cmd": '"$BIN" d', "setup": su, "stdout": o, "match": match} for su, o in zip(setups, outs)]
    tests.append({"cmd": '"$BIN" nodir; echo "exit=$?"', "stdout": "exit=1\n"})
    example = "```\n$ ls d\n" + "".join(x + "\n" for x in listings[0]) + "$ ./prog d\n" + outs[0] + "```\n"
    return {
        "title": title, "stars": 3, "summary": "Read the entries of a folder.",
        "statement": f"`./prog DIR` {what}. If the argument is missing or the folder cannot be opened, print nothing and exit with status 1.",
        "hints": hints, "tests": tests, "example": example,
        "solution": inc + "\nint main(int argc, char *argv[])\n{\n" + body + "    return 0;\n}\n",
    }


def errors_open(r):
    code = r.randint(2, 9)
    pfx = r.choice(["bytecount", "filesize", "fsize", "sizeof"])
    cases = []
    for _ in range(2):
        ls = [" ".join(r.sample(WORDS, r.randint(1, 3))) for _ in range(r.randint(1, 6))]
        cases.append({"cmd": '"$BIN" data.txt', "setup": lines_setup(ls), "stdout": f"{sum(len(l) + 1 for l in ls)}\n"})
    cases.append({"cmd": '"$BIN" data.txt', "setup": ": > data.txt", "stdout": "0\n"})
    cases.append({"cmd": '"$BIN" nofile.txt 2>&1; echo "rc=$?"', "stdout": f"{pfx}: cannot open nofile.txt: No such file or directory\nrc={code}\n"})
    cases.append({"cmd": '"$BIN" 2>&1; echo "rc=$?"', "stdout": f"usage: {pfx} FILE\nrc={code}\n"})
    cases.append({"cmd": '"$BIN" data.txt 2>/dev/null', "setup": lines_setup(["x"]), "stdout": "2\n"})
    return {
        "title": "Report the error", "stars": 2, "summary": "Handle a failing fopen as a real program does.",
        "statement": (f"`./prog FILE` prints the size of FILE in bytes. Errors go to the standard error output, never to the standard output, and the exit status is {code}:\n\n"
                      f"- without exactly one argument: `usage: {pfx} FILE`\n- if the file cannot be opened: `{pfx}: cannot open FILE: <the system's message>` (the message of `errno`, e.g. `No such file or directory`)."),
        "hints": ["`fopen` returns `NULL` on failure and sets `errno`; `strerror(errno)` (from `<string.h>` and `<errno.h>`) is the text of the error.", "`fprintf(stderr, ...)` writes to the error output; the exit status is the value returned by `main` (or passed to `exit`)."],
        "tests": cases, "example": f"```\n$ ./prog nofile.txt\n{pfx}: cannot open nofile.txt: No such file or directory\n$ echo $?\n{code}\n```\n",
        "solution": f"#include <errno.h>\n#include <stdio.h>\n#include <string.h>\n\nint main(int argc, char *argv[])\n{{\n    if (argc != 2) {{\n        fprintf(stderr, \"usage: {pfx} FILE\\n\");\n        return {code};\n    }}\n    FILE *f = fopen(argv[1], \"r\");\n    if (f == NULL) {{\n        fprintf(stderr, \"{pfx}: cannot open %s: %s\\n\", argv[1], strerror(errno));\n        return {code};\n    }}\n    fseek(f, 0, SEEK_END);\n    printf(\"%ld\\n\", ftell(f));\n    fclose(f);\n    return 0;\n}}\n",
    }


def programs_records(r):
    kind = r.choice(["scores", "words"])
    cases = []
    if kind == "scores":
        who, what = r.choice([("player", "points"), ("student", "mark"), ("team", "score")])
        title, summary = f"Best {who}", "Read records until the input ends and summarise them."
        statement = (f"The input has one record per line, `<{who}> <{what}>`: a name (one word of at most 31 characters) and an integer from 0 to 100, until the input ends (at most 50 records, at least 1). "
                     f"Print `best: <{who}> (<{what}>)` for the record with the highest {what} (the first one if several tie) and `average: <average of all the {what}s>` with 2 decimals.")
        for size in (1, r.randint(2, 4), r.randint(4, 9), r.randint(3, 8)):
            names = r.sample(WORDS, size)
            recs = [(nm, r.randint(0, 100)) for nm in names]
            best = recs[0]
            for rec in recs[1:]:
                if rec[1] > best[1]:
                    best = rec
            cases.append({"stdin": "".join(f"{nm} {v}\n" for nm, v in recs),
                          "stdout": f"best: {best[0]} ({best[1]})\naverage: {sum(v for _, v in recs) / len(recs):.2f}\n"})
        solution = ('#include <stdio.h>\n#include <string.h>\n\nint main(void)\n{\n    char name[32], best_name[32] = "";\n    int value, best = -1, count = 0;\n    long total = 0;\n\n'
                    '    while (scanf("%31s %d", name, &value) == 2) {\n        if (value > best) {\n            best = value;\n            strcpy(best_name, name);\n        }\n        total += value;\n        count++;\n    }\n'
                    '    if (count == 0)\n        return 1;\n    printf("best: %s (%d)\\n", best_name, best);\n    printf("average: %.2f\\n", (double)total / count);\n    return 0;\n}\n')
        hints = ["`while (scanf(\"%31s %d\", name, &value) == 2)` reads one record per turn until the input ends.", "Keep the best value and its name while reading, and a running total and count for the average; `strcpy` saves the name."]
    else:
        title, summary = "Most frequent word", "Count words with arrays of strings."
        statement = ("The input has words separated by spaces or line breaks (at least 1 word, at most 50 different ones, each of at most 31 characters). Print the word that appears most often, a space and its count; "
                     "if several words tie, print the one that appeared first in the input.")
        for size in (1, r.randint(4, 9), r.randint(8, 16), r.randint(6, 14)):
            pool = r.sample(WORDS, r.randint(1, 5))
            ws = [r.choice(pool) for _ in range(size)]
            counts, order = {}, []
            for w in ws:
                if w not in counts:
                    order.append(w)
                    counts[w] = 0
                counts[w] += 1
            best = order[0]
            for w in order[1:]:
                if counts[w] > counts[best]:
                    best = w
            lines = [" ".join(ws[i:i + 4]) for i in range(0, len(ws), 4)]
            cases.append({"stdin": "\n".join(lines) + "\n", "stdout": f"{best} {counts[best]}\n"})
        solution = ('#include <stdio.h>\n#include <string.h>\n\nint main(void)\n{\n    char words[50][32];\n    int counts[50];\n    int distinct = 0;\n    char w[32];\n\n'
                    '    while (scanf("%31s", w) == 1) {\n        int i;\n\n        for (i = 0; i < distinct; i++)\n            if (strcmp(words[i], w) == 0)\n                break;\n        if (i == distinct) {\n            strcpy(words[i], w);\n            counts[i] = 0;\n            distinct++;\n        }\n        counts[i]++;\n    }\n'
                    '    if (distinct == 0)\n        return 1;\n    int best = 0;\n    for (int i = 1; i < distinct; i++)\n        if (counts[i] > counts[best])\n            best = i;\n    printf("%s %d\\n", words[best], counts[best]);\n    return 0;\n}\n')
        hints = ["Keep an array of the different words and an array with their counts: for every word read, look it up with `strcmp`.", "A new word is added at the end; replace the best only when a count is strictly greater, so the first one wins ties."]
    return {"title": title, "stars": 4, "summary": summary, "statement": statement, "hints": hints, "tests": cases, "solution": solution}

TEMPLATES = [
    ("t3-tpl-output-frame", "output", output_frame),
    ("t3-tpl-variables-print", "variables", variables_print),
    ("t3-tpl-operators-arith", "operators", operators_arith),
    ("t3-tpl-input-stats", "input", input_stats),
    ("t3-tpl-conditions-classify", "conditions", conditions_classify),
    ("t3-tpl-loops-series", "loops", loops_series),
    ("t3-tpl-arrays-ops", "arrays", arrays_ops),
    ("t3-tpl-strings-ops", "strings", strings_ops),
    ("t3-tpl-pointers-helper", "pointers", pointers_helper),
    ("t3-tpl-memory-dynamic", "memory", memory_dynamic),
    ("t3-tpl-structs-records", "structs", structs_records),
    ("t3-tpl-args-tool", "args", args_tool),
    ("t3-tpl-functions-helper", "functions", functions_helper),
    ("t3-tpl-stdio-lines", "stdio", stdio_lines),
    ("t3-tpl-syscalls-file", "syscalls", syscalls_file),
    ("t3-tpl-directories-list", "directories", directories_list),
    ("t3-tpl-errors-open", "errors", errors_open),
    ("t3-tpl-build-module", "build", build_module),
    ("t3-tpl-programs-records", "programs", programs_records),
]
