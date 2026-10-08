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
from cparse import called_library_functions  # noqa: E402

errors = []
def err(msg):
    errors.append(msg)
    print("  FAIL", msg)


def solution_sources(ex):
    d = Path(ex["dir"])
    if ex.get("files"):
        return [(d / "solution" / n).read_text(encoding="utf-8") for n in ex["files"] if n.endswith((".c", ".h"))]
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
        elif r["log"].strip() and (not ex.get("build") or "warning" in r["log"].lower()):
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
    chapter_ids = {(c["tag"], c["id"]) for c in chapters}
    look = S.reference_lookup()
    ids = set()
    n = 0
    types = {}
    referenced = set()
    for s in theory.values():
        if (s["tag"], s["chapter"]) not in chapter_ids:
            err(f"theory set {s['id']}: chapter '{s['chapter']}' is not a chapter of {s['tag']}")
        if not s["id"].startswith(f"{s['tag']}-th-"):
            err(f"theory set {s['id']}: id must start with '{s['tag']}-th-'")
        for q in s["questions"]:
            n += 1
            qid = q["id"]
            if qid in ids:
                err(f"duplicate question id {qid}")
            ids.add(qid)
            if not qid.startswith(q["tag"] + "-"):
                err(f"{qid}: id must start with '{q['tag']}-'")
            if not (isinstance(q["difficulty"], int) and 1 <= q["difficulty"] <= 5):
                err(f"{qid}: difficulty must be an integer 1-5 (stars)")
            t = q["type"]
            types[t] = types.get(t, 0) + 1
            if t not in ("single", "multiple", "fill", "order", "predict", "match", "sort"):
                err(f"{qid}: unknown type {t}")
                continue
            if not q.get("explain"):
                err(f"{qid}: no explanation")
            for r in q.get("ref", []):
                if str(r).lower() not in look:
                    err(f"{qid}: ref '{r}' is not a reference entry")
                else:
                    referenced.add(look[str(r).lower()]["id"])
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
            if t in ("single", "multiple") and len(set(q["options"])) != len(q["options"]):
                err(f"{qid}: two options are identical")
            if t == "fill":
                markers = len(re.findall(r"\{\{\d+\}\}", q["prompt"] + "\n" + q.get("code", "")))
                if markers != len(q["blanks"]):
                    err(f"{qid}: blanks do not match the {{{{n}}}} markers in the prompt/code")
            if t == "order" and len(q["items"]) < 3:
                err(f"{qid}: order needs 3+ items")
            if t == "match":
                rights = [p[1] for p in q["pairs"]]
                if len(q["pairs"]) < 3 or len(set(rights)) != len(rights) or len({p[0] for p in q["pairs"]}) != len(q["pairs"]):
                    err(f"{qid}: match needs 3+ pairs with distinct left and right sides")
            if t == "sort":
                if len(q["categories"]) < 2 or len(q["items"]) < 4 or any(it[1] not in q["categories"] for it in q["items"]):
                    err(f"{qid}: sort needs 2+ categories, 4+ items, and every item in a listed category")
                if len({it[1] for it in q["items"]}) < 2:
                    err(f"{qid}: sort items all fall in one category")
            if t == "predict":
                if "code" not in q:
                    err(f"{qid}: predict needs a code snippet")
                src = q.get("verify_source", q.get("code", ""))
                with tempfile.TemporaryDirectory() as w:
                    f = Path(w) / "v.c"
                    f.write_text(src)
                    ok, log = S.compile_c([f], Path(w) / "v", w)
                    if not ok:
                        err(f"{qid}: predict program does not compile:\n{log}")
                    else:
                        out = subprocess.run([str(Path(w) / "v")], capture_output=True, text=True, timeout=5, stdin=subprocess.DEVNULL, cwd=w).stdout   # in a scratch folder: snippets may create files
                        if S._output_lines(out) not in [S._output_lines(a) for a in S._answers(q)]:
                            err(f"{qid}: the program prints {out!r} but the answer says {q['answer']!r}")
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
    print(f"theory: {len(theory)} sets, {n} questions {dict(sorted(types.items()))}")
    unref = [e["id"] for e in S.load_reference() if e["id"] not in referenced]
    print(f"  note: {len(unref)} of {len(S.load_reference())} reference entries have no question yet")

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
