#!/usr/bin/env python3
"""Prints, as JSON, what app/server.py makes of a set of path files (the result or the error message): tests/paths.test.mjs checks that src/api.js says the same.
python3 tests/make_path_vectors.py > $TMPDIR/pathvec.json"""
import json
import os
import sys
import tempfile
from pathlib import Path

here = Path(__file__).resolve().parent.parent
app = next(p for p in (here.parent / "app", here.parent / "repo" / "app") if (p / "server.py").exists())
os.environ.setdefault("LAB_PROGRESS", tempfile.mkdtemp())
sys.path.insert(0, str(app))
import server as S  # noqa: E402

ok = "t4-procs-fork-hello"
cases = [
    {"id": "a", "title": "A", "items": [ok]},
    {"id": "a", "title": "A", "items": [ok, ok, "nope"], "blurb": " b "},
    {"id": "p1", "title": " T ", "phases": [{"title": "One", "note": " n ", "items": [ok, "t4-exec-echo"], "theory": ["t4/procs", "x/y"]}, {"items": ["t4-pipes-count-bytes"]}]},
    {"id": "course", "title": "A", "items": [ok]},
    {"id": "Bad", "title": "A", "items": [ok]},
    {"id": "-x", "title": "A", "items": [ok]},
    {"id": "a", "title": "", "items": [ok]},
    {"id": "a", "title": "A"},
    {"id": "a", "title": "A", "items": [ok], "phases": []},
    {"id": "a", "title": "A", "items": ["nope"]},
    {"id": "a", "title": "A", "items": [1]},
    {"id": "a", "title": "A", "phases": [{"title": "x", "items": "no"}]},
    {"id": "a", "title": "A", "phases": [{"title": "x", "items": [ok], "theory": "t4/procs"}]},
    {"id": "a", "title": "A", "phases": [{"title": "x" * 81, "items": [ok]}]},
    {"id": "a", "title": "A", "phases": []},
    [1],
    "text",
]
coding, chapters, taken = S.load_coding(), S.load_chapters(), S.builtin_path_ids()
out = []
for c in cases:
    try:
        p, w = S.normalize_custom_path(c, coding, chapters, taken)
        out.append({"in": c, "path": p, "warnings": w})
    except ValueError as e:
        out.append({"in": c, "error": str(e)})
json.dump({"cases": out, "coding": sorted(coding), "chapters": [f"{c['tag']}/{c['id']}" for c in chapters], "taken": sorted(taken)}, sys.stdout)
