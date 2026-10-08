#!/usr/bin/env python3
"""Self-test for the content: every reference solution must pass its own checks, every (blank) starter must fail,
ids/tags/chapters/path must be consistent, every library function used by a solution must have a reference entry,
and theory/reference JSON must be well-formed. Run with `./lab selftest`."""
import json
import re
import subprocess
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import server as S  # noqa: E402

errors = []
KEYWORDS = {"if", "while", "for", "switch", "return", "sizeof", "else", "do", "case", "defined", "typeof", "main"}


def err(msg):
    errors.append(msg)
    print("  FAIL", msg)


def strip_comments(src):
    src = re.sub(r"/\*.*?\*/", " ", src, flags=re.S)
    src = re.sub(r"//[^\n]*", " ", src)
    return re.sub(r'"(\\.|[^"\\])*"', '""', src)


def called_library_functions(sources):
    """Identifiers used as `name(` that the exercise does not define itself (so: library calls)."""
    text = "\n".join(strip_comments(s) for s in sources)
    defined = set(re.findall(r"^[A-Za-z_][\w\s\*]*?\b([A-Za-z_]\w*)\s*\([^;{}]*\)\s*\{", text, flags=re.M))
    defined |= set(re.findall(r"^[A-Za-z_][\w\s\*]*?\b([A-Za-z_]\w*)\s*\([^;{}]*\)\s*;", text, flags=re.M))  # prototypes
    called = set(re.findall(r"\b([A-Za-z_]\w*)\s*\(", text))
    # function-pointer parameters / local callbacks: `int (*cmp)(...)`, `cmp(`
    callbacks = set(re.findall(r"\(\s*\*\s*(\w+)\s*\)\s*\(", text))
    return called - KEYWORDS - defined - callbacks


def solution_sources(ex):
    d = Path(ex["dir"])
    if ex.get("files"):
        return [(d / "solution" / n).read_text(encoding="utf-8") for n in ex["files"]]
    return [(d / "solution.c").read_text(encoding="utf-8")]


def main():
    chapters = S.load_chapters()
    chap = {(c["tag"], c["id"]): c for c in chapters}
    coding = S.load_coding()
    print(f"chapters: {len(chapters)}  coding exercises: {len(coding)}")
    ref = S.load_reference()
    ref_names = set()
    for e in ref:
        ref_names.add(e["title"].split()[0].lower())
        ref_names.add(e["id"].split("-", 2)[-1].lower())
        for a in e.get("aliases", []):
            ref_names.add(a.lower())
    missing_ref = {}
    for ex in coding.values():
        d = Path(ex["dir"])
        for f in ("statement.md", "tests.json", "meta.json"):
            if not (d / f).exists():
                err(f"{ex['id']}: missing {f}")
        has_solution = (d / "solution.c").exists() or (ex.get("files") and all((d / "solution" / n).exists() for n in ex["files"]))
        if not has_solution:
            err(f"{ex['id']}: missing solution")
            continue
        for k in ("id", "tag", "track", "topic", "title", "summary"):
            if k not in ex:
                err(f"{ex['id']}: meta lacks {k}")
        if not isinstance(ex["stars"], int) or not 1 <= ex["stars"] <= 5:
            err(f"{ex['id']}: stars must be an integer 1-5")
        c = chap.get((ex["tag"], ex["topic"]))
        if not c:
            err(f"{ex['id']}: topic '{ex['topic']}' is not a chapter of {ex['tag']}")
        elif c["track"] != ex["track"]:
            err(f"{ex['id']}: track {ex['track']} differs from its chapter's track {c['track']}")
        if not ex["id"].startswith(f"{ex['tag']}-{ex['topic']}-"):
            err(f"{ex['id']}: id must start with '{ex['tag']}-{ex['topic']}-'")
        if d.parts[-4] != ex["tag"] or d.parts[-2] != ex["topic"]:
            err(f"{ex['id']}: must live in content/{ex['tag']}/coding/{ex['topic']}/")
        if len(ex.get("hints", [])) < 2:
            err(f"{ex['id']}: needs at least 2 hints")
        if (d / "starter.c").exists() and (d / "starter.c").read_text().strip():
            err(f"{ex['id']}: starters must be blank (no fill-in exercises); remove starter.c")
        r = S.check_solution(ex)
        bad = [x["name"] for x in r["cases"] if not x["ok"]]
        if not r["compiled"]:
            err(f"{ex['id']}: solution does not compile:\n{r['log']}")
        elif bad:
            err(f"{ex['id']}: solution fails: {bad}")
        elif r["log"].strip():
            err(f"{ex['id']}: solution compiles with warnings:\n{r['log']}")
        else:
            print(f"  ok   {ex['id']}: passes {len(r['cases'])} cases")
        # blank starter must not pass
        saved = S.WORKSPACE
        with tempfile.TemporaryDirectory() as w:
            S.WORKSPACE = Path(w)
            S.ensure_workspace(ex)
            r = S.check_exercise(ex)
            S.WORKSPACE = saved
        if r["passed"]:
            err(f"{ex['id']}: an empty answer passes")
        for fn in called_library_functions(solution_sources(ex)):
            if fn.lower() not in ref_names:
                missing_ref.setdefault(fn, []).append(ex["id"])
    for fn, ids in sorted(missing_ref.items()):
        err(f"library function '{fn}' used by {ids[0]}{' (+%d more)' % (len(ids) - 1) if len(ids) > 1 else ''} has no reference entry")

    # chapters and suggested path
    for tag, ids in S.load_paths().items():
        known = {e["id"] for e in coding.values() if e["tag"] == tag}
        if len(ids) != len(set(ids)):
            err(f"path of {tag} lists an exercise twice")
        for i in ids:
            if i not in known:
                err(f"path of {tag} lists unknown exercise {i}")
        for i in known - set(ids):
            err(f"exercise {i} is not on the path of {tag}")
        stars = [coding[i]["stars"] for i in ids if i in coding]
        for k in range(1, len(stars)):
            if stars[k] > max(stars[max(0, k - 8):k]) + 1:
                err(f"path of {tag}: stars jump to {stars[k]} at {ids[k]}")
    for (tag, cid), c in chap.items():
        n = sum(1 for e in coding.values() if e["tag"] == tag and e["topic"] == cid)
        if n == 0:
            print(f"  note: chapter {tag}/{cid} has no exercises yet")

    theory = S.load_theory()
    ids = set()
    n = 0
    for s in theory.values():
        for q in s["questions"]:
            n += 1
            qid = q["id"]
            if qid in ids:
                err(f"duplicate question id {qid}")
            ids.add(qid)
            if not qid.startswith(q["tag"] + "-"):
                err(f"{qid}: id must start with '{q['tag']}-'")
            t = q["type"]
            if t not in ("single", "multiple", "fill", "order"):
                err(f"{qid}: unknown type {t}")
                continue
            if not q.get("explain"):
                err(f"{qid}: no explanation")
            if t == "single":
                if not (isinstance(q["answer"], int) and 0 <= q["answer"] < len(q["options"])):
                    err(f"{qid}: bad answer index")
                if len(q["options"]) < 3:
                    err(f"{qid}: needs at least 3 options")
            if t == "multiple":
                if not q["answer"] or any(not (0 <= a < len(q["options"])) for a in q["answer"]):
                    err(f"{qid}: bad answer indices")
                if len(q["answer"]) < 2:
                    err(f"{qid}: a 'multiple' question needs 2+ correct options")
            if t in ("single", "multiple") and q.get("option_notes") and len(q["option_notes"]) != len(q["options"]):
                err(f"{qid}: option_notes length differs from options")
            if t == "fill":
                if len(re.findall(r"\{\{\d+\}\}", q["prompt"])) != len(q["blanks"]):
                    err(f"{qid}: blanks do not match the {{{{n}}}} markers in the prompt")
            if t == "order" and len(q["items"]) < 3:
                err(f"{qid}: order needs 3+ items")
            if "verify" in q:  # {"source": "...C program...", "stdout": "..."} proves a code-output claim
                with tempfile.TemporaryDirectory() as w:
                    src = Path(w) / "v.c"
                    src.write_text(q["verify"]["source"])
                    ok, log = S.compile_c([src], Path(w) / "v", w)
                    if not ok:
                        err(f"{qid}: verify program does not compile:\n{log}")
                    else:
                        out = subprocess.run([str(Path(w) / "v")], capture_output=True, text=True, timeout=5).stdout
                        if out != q["verify"]["stdout"]:
                            err(f"{qid}: verify output {out!r} != {q['verify']['stdout']!r}")
    print(f"theory: {len(theory)} sets, {n} questions")

    print(f"reference entries: {len(ref)}")
    rid = set()
    owner = {}
    for e in ref:
        for k in ("id", "title", "category", "summary"):
            if k not in e:
                err(f"reference {e.get('id', '?')}: lacks {k}")
        if e.get("id") in rid:
            err(f"duplicate reference id {e['id']}")
        rid.add(e.get("id"))
        if not str(e.get("id", "")).startswith(e["tag"] + "-"):
            err(f"reference {e.get('id')}: id must start with '{e['tag']}-'")
        for a in [e["title"].split()[0]] + e.get("aliases", []):
            if owner.setdefault(a.lower(), e["id"]) != e["id"]:
                err(f"reference name/alias '{a}' belongs to both {owner[a.lower()]} and {e['id']}")
        for s in e.get("see", []):
            if s not in {x["id"] for x in ref}:
                err(f"reference {e['id']}: see-also {s} does not exist")
    if errors:
        print(f"\n{len(errors)} problem(s)")
        return 1
    print("\nall good")
    return 0


if __name__ == "__main__":
    sys.exit(main())
