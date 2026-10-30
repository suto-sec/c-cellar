#!/usr/bin/env python3
"""Vectors for tests/grade.test.mjs: for every theory question, a few responses and what app/server.py's grade() says about each."""
import json, os, random, sys, tempfile
from pathlib import Path
os.environ.setdefault("LAB_PROGRESS", tempfile.mkdtemp(prefix="cellar-vec-"))
sys.path.insert(0, str(Path(__file__).resolve().parents[2] / "repo" / "app"))
import server as S

rng = random.Random(7)
out = []
for s in S.load_theory().values():
    for q in s["questions"]:
        t = q["type"]
        resp = []
        if t == "single":
            resp = [q["answer"], (q["answer"] + 1) % len(q["options"]), "x", None]
        elif t == "multiple":
            a = list(q["answer"])
            resp = [a, list(reversed(a)), a[:-1], a + [i for i in range(len(q["options"])) if i not in a][:1], []]
        elif t == "order":
            n = len(q["items"]); ok = list(range(n)); bad = ok[::-1]
            resp = [ok, bad, ok[:-1]]
        elif t == "fill":
            good = [(b["accept"] if isinstance(b, dict) else b)[0].removeprefix("re:") for b in q["blanks"]]
            resp = [good, [g.upper() for g in good], ["  " + g + "  " for g in good], ["zzz"] * len(good), good[:-1]]
        elif t == "predict":
            a = q["answer"] if isinstance(q["answer"], str) else q["answer"][0]
            resp = [a, a + "\n\n", a.upper(), "   " + a.replace("\n", "  \n"), "nothing", ""]
        elif t in ("match", "sort"):
            right = [p[1] for p in (q["pairs"] if t == "match" else q["items"])]
            shuffled = right[:]; rng.shuffle(shuffled)
            resp = [right, shuffled, right[:-1]]
        for r in resp:
            out.append({"q": {k: v for k, v in q.items() if k in ("id", "type", "answer", "options", "items", "pairs", "blanks", "categories")}, "response": r, "grade": S.grade(q, r),
                        "blanks_ok": S.blanks_ok(q, r) if t == "fill" else None, "items_ok": S.pairs_ok(q, r) if t in ("match", "sort") else None,
                        "hints": S.predict_hints(q, r) if t == "predict" else None})
print(json.dumps(out))
