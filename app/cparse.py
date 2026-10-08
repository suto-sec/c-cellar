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
