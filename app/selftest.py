#!/usr/bin/env python3
"""Self-test for the content: every reference solution must pass its own checks, every starter must not,
and theory/reference JSON must be well-formed and tagged. Run with `./lab selftest`."""
import json
import re
import shutil
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import server as S  # noqa: E402

errors = []


def err(msg):
    errors.append(msg)
    print("  FAIL", msg)


def main():
    coding = S.load_coding()
    print(f"coding exercises: {len(coding)}")
    for ex in coding.values():
        d = Path(ex["dir"])
        for f in ("statement.md", "starter.c", "solution.c", "tests.json", "meta.json"):
            if not (d / f).exists():
                err(f"{ex['id']}: missing {f}")
        for k in ("id", "tag", "track", "topic", "title"):
            if k not in ex:
                err(f"{ex['id']}: meta lacks {k}")
        if not ex["id"].startswith(ex["tag"] + "-"):
            err(f"{ex['id']}: id must start with its tag '{ex['tag']}-'")
        if d.parts[-4] != ex["tag"]:
            err(f"{ex['id']}: lives under content/{d.parts[-4]}/ but is tagged {ex['tag']}")
        if (d / "solution.c").exists():
            r = S.check_solution(ex)
            bad = [c["name"] for c in r["cases"] if not c["ok"]]
            if not r["compiled"]:
                err(f"{ex['id']}: solution does not compile:\n{r['log']}")
            elif bad:
                err(f"{ex['id']}: solution fails: {bad}")
            elif r["log"].strip():
                err(f"{ex['id']}: solution compiles with warnings:\n{r['log']}")
            else:
                print(f"  ok   {ex['id']}: solution passes {len(r['cases'])} cases")
        # starter: must compile and must not pass everything
        saved = S.WORKSPACE
        with tempfile.TemporaryDirectory() as w:
            S.WORKSPACE = Path(w)
            S.ensure_workspace(ex)
            r = S.check_exercise(ex)
            S.WORKSPACE = saved
        if r["passed"]:
            err(f"{ex['id']}: the starter already passes")
        elif not r["compiled"]:
            err(f"{ex['id']}: starter does not compile:\n{r['log']}")

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
                        import subprocess
                        out = subprocess.run([str(Path(w) / "v")], capture_output=True, text=True, timeout=5).stdout
                        if out != q["verify"]["stdout"]:
                            err(f"{qid}: verify output {out!r} != {q['verify']['stdout']!r}")
    print(f"theory: {len(theory)} sets, {n} questions")

    ref = S.load_reference()
    print(f"reference entries: {len(ref)}")
    rid = set()
    for e in ref:
        for k in ("id", "title", "category", "summary"):
            if k not in e:
                err(f"reference {e.get('id', '?')}: lacks {k}")
        if e.get("id") in rid:
            err(f"duplicate reference id {e['id']}")
        rid.add(e.get("id"))
        if not str(e.get("id", "")).startswith(e["tag"] + "-"):
            err(f"reference {e.get('id')}: id must start with '{e['tag']}-'")
    if errors:
        print(f"\n{len(errors)} problem(s)")
        return 1
    print("\nall good")
    return 0


if __name__ == "__main__":
    sys.exit(main())
