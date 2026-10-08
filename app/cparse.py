"""Tiny helpers to see which library functions a C source calls (used by the server and by selftest)."""
import re

KEYWORDS = {"if", "while", "for", "switch", "return", "sizeof", "else", "do", "case", "defined", "typeof", "main"}


def strip_comments(src):
    src = re.sub(r"/\*.*?\*/", " ", src, flags=re.S)
    src = re.sub(r"//[^\n]*", " ", src)
    return re.sub(r'"(\\.|[^"\\])*"', '""', src)


def called_library_functions(sources):
    """Identifiers used as `name(` that the sources do not define themselves (so: library calls)."""
    text = "\n".join(strip_comments(s) for s in sources)
    defined = set(re.findall(r"^[A-Za-z_][\w\s\*]*?\b([A-Za-z_]\w*)\s*\([^;{}]*\)\s*\{", text, flags=re.M))
    defined |= set(re.findall(r"^[A-Za-z_][\w\s\*]*?\b([A-Za-z_]\w*)\s*\([^;{}]*\)\s*;", text, flags=re.M))  # prototypes
    called = set(re.findall(r"\b([A-Za-z_]\w*)\s*\(", text))
    callbacks = set(re.findall(r"\(\s*\*\s*(\w+)\s*\)\s*\(", text))  # function-pointer parameters
    return called - KEYWORDS - defined - callbacks


FORMAT = re.compile(r"%[-+ #0]*\d*(?:\.\d+)?(hh|h|ll|l|z|j|t|L)?([diouxXfeEgGcsp%])")


def used_symbols(sources):
    """Everything an entry of the reference may be named after in the sources: identifiers (keywords and library names
    included), `#directives`, header names, `->` and printf/scanf conversions such as `%zu`."""
    out = set()
    for src in sources:
        out |= {"%" + (m.group(1) or "") + m.group(2) for lit in re.findall(r'"((?:\\.|[^"\\])*)"', re.sub(r"/\*.*?\*/|//[^\n]*", " ", src, flags=re.S)) for m in FORMAT.finditer(lit)}
        out |= set(re.findall(r"#\s*include\s*[<\"]([\w./]+)[>\"]", src))
        text = strip_comments(src)
        out |= {"#" + d for d in re.findall(r"^\s*#\s*(\w+)", text, flags=re.M)}
        out |= set(re.findall(r"[A-Za-z_]\w*", text))
        if "->" in text:
            out.add("->")
    return out
