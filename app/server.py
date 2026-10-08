#!/usr/bin/env python3
"""c-cellar backend: serves the UI, loads content, checks C exercises, grades theory answers, tracks progress.

Standard library only. Content lives in content/<tag>/ (tag = course topic, e.g. t3); progress in .progress/.
"""
import json
import os
import random
import re
import shlex
import shutil
import signal
import subprocess
import sys
import tempfile
import threading
import time
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer
from pathlib import Path
from urllib.parse import unquote, urlparse

from cparse import called_library_functions

ROOT = Path(os.environ.get("LAB_ROOT", Path(__file__).resolve().parent.parent))
CONTENT = ROOT / "content"
PROGRESS_DIR = Path(os.environ.get("LAB_PROGRESS", ROOT / ".progress"))
WORKSPACE = PROGRESS_DIR / "workspace"
STATIC = Path(__file__).resolve().parent / "static"
VSCODE_PORT = int(os.environ.get("LAB_VSCODE_PORT", "8081"))
CC = os.environ.get("LAB_CC", "gcc")
LOCK = threading.Lock()

# ------------------------------------------------------------------ content loading


def _read_json(path):
    with open(path, encoding="utf-8") as f:
        return json.load(f)


def load_coding():
    """{id: exercise dict} for every content/<tag>/coding/<track>/<id>/meta.json."""
    out = {}
    for meta in sorted(CONTENT.glob("*/coding/*/*/meta.json")):
        d = meta.parent
        m = _read_json(meta)
        m["dir"] = str(d)
        m.setdefault("stars", m.get("difficulty", 1))
        out[m["id"]] = m
    return out


def load_chapters():
    """Chapters (topics) of every tag, in teaching order: content/<tag>/chapters.json."""
    out = []
    for f in sorted(CONTENT.glob("*/chapters.json")):
        for i, c in enumerate(_read_json(f)["chapters"]):
            c["tag"] = f.parent.name
            c["order"] = i
            out.append(c)
    return out


def load_paths():
    """{tag: [exercise ids]}: the suggested path, content/<tag>/path.json."""
    out = {}
    for f in sorted(CONTENT.glob("*/path.json")):
        out[f.parent.name] = _read_json(f)
    return out


def load_theory():
    """{id: set}. A set mirrors one chapter (`chapter`); questions inside a set are ordered by difficulty."""
    order = {(c["tag"], c["id"]): c["order"] for c in load_chapters()}
    sets = []
    for f in sorted(CONTENT.glob("*/theory/*.json")):
        s = _read_json(f)
        s.setdefault("chapter", s.get("topic", ""))
        for q in s["questions"]:
            q.setdefault("topic", s["chapter"])
            q.setdefault("tag", s["tag"])
            q.setdefault("difficulty", 1)
        s["questions"].sort(key=lambda q: q["difficulty"])  # stable: keeps the written order inside a level
        sets.append(s)
    sets.sort(key=lambda s: (order.get((s["tag"], s["chapter"]), 99), s["id"]))
    return {s["id"]: s for s in sets}


def load_reference():
    files = sorted(((_read_json(f), f) for f in CONTENT.glob("*/reference/*.json")), key=lambda p: (p[0].get("order", 99), p[1].name))
    out = []
    for data, f in files:
        for e in data["entries"]:
            e.setdefault("tag", f.parent.parent.name)
            out.append(e)
    return out


# ------------------------------------------------------------------ progress


def _progress_file():
    return PROGRESS_DIR / "progress.json"


def load_progress():
    try:
        return _read_json(_progress_file())
    except (OSError, ValueError):
        return {"coding": {}, "theory": {}}


def save_progress(p):
    PROGRESS_DIR.mkdir(parents=True, exist_ok=True)
    tmp = _progress_file().with_suffix(".tmp")
    tmp.write_text(json.dumps(p, indent=1), encoding="utf-8")
    os.replace(tmp, _progress_file())


def coding_status(entry):
    if not entry:
        return "todo"
    if entry.get("passed"):
        return "passed"
    if entry.get("solution_viewed"):
        return "solution"
    if entry.get("attempts"):
        return "progress"
    return "todo"


# ------------------------------------------------------------------ coding: workspace + checker


def workspace_dir(ex_id):
    return WORKSPACE / ex_id


def workspace_files(ex):
    """Files the student edits: answer.c, or the named files of a multi-file exercise."""
    return ex.get("files") or ["answer.c"]


def ensure_workspace(ex):
    d = workspace_dir(ex["id"])
    d.mkdir(parents=True, exist_ok=True)
    exd = Path(ex["dir"])
    for name in workspace_files(ex):
        f = d / name
        if f.exists():
            continue
        src = exd / "starter" / name
        if not src.exists() and name == "answer.c":
            src = exd / "starter.c"
        if src.exists():
            shutil.copyfile(src, f)
        else:
            f.write_text("", encoding="utf-8")  # starters are blank: you write the whole program
    for h in exd.glob("*.h"):  # headers the statement refers to, visible in the editor
        if not (d / h.name).exists():
            shutil.copyfile(h, d / h.name)
    return d


# CPU seconds, max file size (KB), address space (KB). Applied in the child shell, not via preexec_fn (unsafe with threads).
ULIMIT = "ulimit -t 10 -f 20480 -v 1048576 2>/dev/null; "


def _clean(text, tmp):
    return text.replace(str(tmp) + "/", "").replace(str(tmp), ".")


def compile_c(sources, out, cwd, extra=()):
    cmd = [CC, "-std=gnu17", "-Wall", "-Wextra", "-g", "-o", str(out), *map(str, sources), *extra, "-lm"]
    p = subprocess.run(cmd, cwd=cwd, capture_output=True, text=True, timeout=60)
    return p.returncode == 0, _clean(p.stderr, cwd)


def run_case(case, binary, fixtures, tmp, idx):
    cdir = tmp / f"case{idx}"
    if fixtures.is_dir():
        shutil.copytree(fixtures, cdir)
    else:
        cdir.mkdir()
    if (cdir / binary.name).exists():  # multi-file exercises: run the copy of the build next to its libraries and objects
        binary = cdir / binary.name
    env = {"PATH": os.environ.get("PATH", "/usr/bin:/bin"), "HOME": str(cdir), "BIN": str(binary),
           "LANG": "C.UTF-8", "TMPDIR": str(cdir)}
    env.update(case.get("env", {}))
    if "setup" in case:
        subprocess.run(["bash", "-c", case["setup"]], cwd=cdir, env=env, capture_output=True, timeout=10)
    if "cmd" in case:
        shown = case["cmd"].replace('"$BIN"', "./prog").replace("$BIN", "./prog")
        argv = ["bash", "-c", ULIMIT + case["cmd"]]
    else:
        args = [str(a) for a in case.get("args", [])]
        shown = " ".join(["./prog"] + [shlex.quote(a) for a in args])
        argv = ["bash", "-c", ULIMIT + 'exec "$@"', "bash", str(binary)] + args
    if "stdin" in case:
        shown += "   < (stdin below)"
    res = {"name": case.get("name", shown), "cmd": shown, "stdin": case.get("stdin"), "hidden": bool(case.get("hidden"))}
    timeout = case.get("timeout", 5)
    try:
        p = subprocess.Popen(argv, cwd=cdir, env=env, stdin=subprocess.PIPE, stdout=subprocess.PIPE,
                             stderr=subprocess.PIPE, text=True, errors="replace", start_new_session=True)
    except OSError as e:
        res.update(ok=False, got="", hints=[f"could not run the program: {e}"])
        return res
    try:
        out, err = p.communicate(case.get("stdin", ""), timeout=timeout)
    except subprocess.TimeoutExpired:
        try:
            os.killpg(p.pid, signal.SIGKILL)
        except OSError:
            p.kill()
        out, err = p.communicate()
        res.update(ok=False, got=out[:2000], stderr=err[:1000], timeout=True,
                   hints=[f"timed out after {timeout}s (infinite loop, or waiting for input that never comes?)"])
        return res
    code = p.returncode
    expected = case.get("stdout", "")
    mode = case.get("match", "exact")
    ok_out, hints = compare(out, expected, mode)
    exp_exit = case.get("exit", 0)
    ok_exit = True
    if exp_exit == "any":
        pass
    elif exp_exit == "nonzero":
        ok_exit = code != 0
    else:
        ok_exit = code == exp_exit
    if not ok_exit:
        if code < 0:
            hints.append(f"the program was killed by signal {signal.Signals(-code).name}"
                         + (" (invalid memory access)" if -code == signal.SIGSEGV else ""))
        else:
            hints.append(f"exit status was {code}, expected {exp_exit}")
    if case.get("stderr_empty") and err.strip():
        hints.append("anything that is not the result (errors) should go to stderr, and a correct run prints no errors")
    if not ok_out and not out.strip() and err.strip():
        hints.append("nothing on stdout, but something on stderr: results go to stdout (printf), not stderr")
    ok = ok_out and ok_exit and not (case.get("stderr_empty") and err.strip())
    res.update(ok=ok, got=out[:4000], exit=code, stderr=err[:1000], hints=hints, match=mode)
    if not res["hidden"]:
        res["expected"] = expected if mode != "regex" else f"(matches /{expected}/)"
    return res


def compare(got, want, mode):
    hints = []
    if mode == "exact":
        if got == want:
            return True, hints
        if got.rstrip() == want.rstrip():
            hints.append("only trailing whitespace/newlines differ (missing or extra final newline?)")
        elif [l.rstrip() for l in got.splitlines()] == [l.rstrip() for l in want.splitlines()]:
            hints.append("only trailing spaces at the end of lines differ")
        elif got.split() == want.split():
            hints.append("same words, different spacing or line breaks")
        elif sorted(got.splitlines()) == sorted(want.splitlines()):
            hints.append("same lines, but in a different order")
        elif got.lower() == want.lower():
            hints.append("differs only in upper/lower case")
        return False, hints
    if mode == "trim":
        ok = got.strip() == want.strip()
        return ok, hints
    if mode == "sorted":
        ok = sorted(got.splitlines()) == sorted(want.splitlines())
        return ok, hints
    if mode == "regex":
        return re.fullmatch(want, got, re.S) is not None, hints
    if mode == "contains":
        return want in got, hints
    return got == want, hints


def check_exercise(ex):
    d = Path(ex["dir"])
    ws = ensure_workspace(ex)
    tests = _read_json(d / "tests.json")
    names = workspace_files(ex)
    empty = [n for n in names if not (ws / n).read_text(encoding="utf-8", errors="replace").strip()]
    if empty and len(empty) == len(names):
        return {"compiled": False, "log": f"{', '.join(empty)} is empty: write your program there first.", "cases": [], "passed": False}
    with tempfile.TemporaryDirectory(prefix="cellar-") as t:
        tmp = Path(t)
        for n in names:
            shutil.copyfile(ws / n, tmp / n)
        for h in d.glob("*.h"):  # always the original header, whatever happened to the workspace copy
            shutil.copyfile(h, tmp / h.name)
        for hidden in (d / "hidden").glob("*") if (d / "hidden").is_dir() else []:  # files the student is linked with (e.g. a main.c)
            shutil.copyfile(hidden, tmp / hidden.name)
        out = tmp / "prog"
        try:
            if ex.get("build"):  # multi-file exercise: its own build command must produce ./prog
                p = subprocess.run(["bash", "-c", ex["build"]], cwd=tmp, capture_output=True, text=True, timeout=60)
                ok, log = p.returncode == 0 and out.exists(), _clean(p.stdout + p.stderr, tmp)
                if p.returncode == 0 and not out.exists():
                    log += "\nThe build finished but did not create ./prog."
            else:
                sources = [tmp / "answer.c"]
                if (d / "harness.c").exists():
                    shutil.copyfile(d / "harness.c", tmp / "harness.c")
                    sources.append(tmp / "harness.c")
                ok, log = compile_c(sources, out, tmp)
        except subprocess.TimeoutExpired:
            return {"compiled": False, "log": "the build timed out", "cases": [], "passed": False}
        if not ok:
            return {"compiled": False, "log": log, "cases": [], "passed": False}
        cases = []
        fixtures = d / "fixtures"
        if ex.get("build"):  # every case starts from a copy of the finished build directory (timestamps kept)
            built = tmp / "_build"
            built.mkdir()
            for f in tmp.iterdir():
                if f.is_file():
                    shutil.copy2(f, built / f.name)
            fixtures = built
        for i, c in enumerate(tests["cases"]):
            cases.append(run_case(c, out, fixtures, tmp, i))
    passed = all(c["ok"] for c in cases)
    return {"compiled": True, "log": log, "cases": cases, "passed": passed, "build": bool(ex.get("build"))}


def solution_text(ex):
    d = Path(ex["dir"])
    if ex.get("files"):
        return "\n".join(f"/* ===== {n} ===== */\n" + (d / "solution" / n).read_text(encoding="utf-8") for n in ex["files"])
    return (d / "solution.c").read_text(encoding="utf-8")


def check_solution(ex):
    """Used by selftest: run the reference solution through the checker in a scratch workspace."""
    global WORKSPACE
    saved = WORKSPACE
    with tempfile.TemporaryDirectory(prefix="cellar-ws-") as w:
        WORKSPACE = Path(w)
        d = workspace_dir(ex["id"])
        d.mkdir(parents=True)
        if ex.get("files"):
            for n in ex["files"]:
                shutil.copyfile(Path(ex["dir"]) / "solution" / n, d / n)
        else:
            shutil.copyfile(Path(ex["dir"]) / "solution.c", d / "answer.c")
        try:
            return check_exercise(ex)
        finally:
            WORKSPACE = saved


# ------------------------------------------------------------------ theory grading


def _norm(s, ignore_space=False, case=False):
    s = s.strip()
    s = re.sub(r"\s+", "" if ignore_space else " ", s)
    return s if case else s.lower()


def blanks_ok(q, response):
    """One boolean per blank of a fill-in question."""
    if not isinstance(response, list) or len(response) != len(q["blanks"]):
        return [False] * len(q["blanks"])
    out = []
    for given, blank in zip(response, q["blanks"]):
        accepted = blank["accept"] if isinstance(blank, dict) else blank
        ign = blank.get("ignore_space", False) if isinstance(blank, dict) else False
        case = blank.get("case", False) if isinstance(blank, dict) else False
        g = _norm(str(given), ign, case)
        hit = False
        for a in accepted:
            if a.startswith("re:"):
                hit = re.fullmatch(a[3:], str(given).strip(), 0 if case else re.I) is not None
            else:
                hit = g == _norm(a, ign, case)
            if hit:
                break
        out.append(hit)
    return out


def _output_lines(text):
    """What counts as 'the same output': trailing spaces and trailing blank lines do not matter."""
    lines = [l.rstrip() for l in str(text).replace("\r", "").split("\n")]
    while lines and not lines[-1]:
        lines.pop()
    return lines


def _answers(q):
    a = q["answer"]
    return a if isinstance(a, list) else [a]


def pairs_ok(q, response):
    """One boolean per item of a match / sort question."""
    right = [p[1] for p in (q["pairs"] if q["type"] == "match" else q["items"])]
    if not isinstance(response, list) or len(response) != len(right):
        return [False] * len(right)
    return [given == want for given, want in zip(response, right)]


def grade(q, response):
    t = q["type"]
    if t == "single":
        return response == q["answer"]
    if t == "multiple":
        return isinstance(response, list) and sorted(response) == sorted(q["answer"])
    if t == "order":
        return response == list(range(len(q["items"])))
    if t == "fill":
        return all(blanks_ok(q, response))
    if t == "predict":
        return _output_lines(response) in [_output_lines(a) for a in _answers(q)]
    if t in ("match", "sort"):
        return all(pairs_ok(q, response))
    return False


def predict_hints(q, response):
    got, want = str(response), _answers(q)[0]
    if not got.strip():
        return ["type what the program prints, exactly (several lines are allowed)"]
    hints = []
    if got.lower().split() == want.lower().split() and got.split() != want.split():
        hints.append("right text, but upper/lower case differs")
    elif got.split() == want.split():
        hints.append("same words, but the spacing or the line breaks differ")
    elif len(_output_lines(got)) != len(_output_lines(want)):
        hints.append(f"the program prints {len(_output_lines(want))} line(s); you typed {len(_output_lines(got))}")
    return hints


def public_question(q):
    """Question without the answer, for the client."""
    out = {k: q[k] for k in ("id", "type", "topic", "tag", "prompt", "code", "difficulty") if k in q}
    if q["type"] in ("single", "multiple"):
        out["options"] = q["options"]
    elif q["type"] == "order":
        idx = list(range(len(q["items"])))
        while True:
            random.shuffle(idx)
            if idx != sorted(idx) or len(idx) < 2:
                break
        out["items"] = [{"id": i, "text": q["items"][i]} for i in idx]
    elif q["type"] == "fill":
        out["blanks"] = len(q["blanks"])
    elif q["type"] == "match":
        out["items"] = [p[0] for p in q["pairs"]]
        opts = [p[1] for p in q["pairs"]]
        random.shuffle(opts)
        out["options"] = opts
    elif q["type"] == "sort":
        out["items"] = [it[0] for it in q["items"]]
        out["categories"] = q["categories"]
    return out


def answer_reveal(q):
    out = {"explain": q.get("explain", "")}
    t = q["type"]
    if t in ("single", "multiple"):
        out["answer"] = q["answer"]
        out["option_notes"] = q.get("option_notes")
    elif t == "order":
        out["answer"] = q["items"]
    elif t == "fill":
        out["answer"] = [(b["accept"] if isinstance(b, dict) else b)[0].removeprefix("re:") for b in q["blanks"]]
    elif t == "predict":
        out["answer"] = _answers(q)[0]
    elif t == "match":
        out["answer"] = [p[1] for p in q["pairs"]]
    elif t == "sort":
        out["answer"] = [it[1] for it in q["items"]]
    return out


# ------------------------------------------------------------------ links between theory, reference and exercises


def reference_lookup():
    """Lower-case id, name or alias -> reference entry."""
    out = {}
    for e in load_reference():
        out[e["id"].lower()] = e
        out[e["title"].split()[0].lower()] = e
        for a in e.get("aliases", []):
            out.setdefault(a.lower(), e)
    return out


def resolve_refs(names):
    look = reference_lookup()
    seen, out = set(), []
    for n in names or []:
        e = look.get(str(n).lower())
        if e and e["id"] not in seen:
            seen.add(e["id"])
            out.append({"id": e["id"], "title": e["title"]})
    return out


def chapter_practice(tag, chapter):
    """The coding chapter that a theory set mirrors: where to practise it (None if it has no exercises)."""
    coding = [e for e in load_coding().values() if e["tag"] == tag and e["topic"] == chapter]
    ch = next((c for c in load_chapters() if c["tag"] == tag and c["id"] == chapter), None)
    if not coding or not ch:
        return None
    return {"chapter": chapter, "tag": tag, "title": ch["title"], "count": len(coding)}


def practice_for_entries():
    """{reference id: [exercise ids that call it]}, in suggested-path order."""
    look = reference_lookup()
    coding = load_coding()
    pos = {}
    for ids in load_paths().values():
        for i, eid in enumerate(ids):
            pos.setdefault(eid, i)
    out = {}
    for eid, ex in sorted(coding.items(), key=lambda kv: (pos.get(kv[0], 10**6), kv[0])):
        d = Path(ex["dir"])
        files = [d / "solution" / n for n in ex["files"] if n.endswith((".c", ".h"))] if ex.get("files") else [d / "solution.c"]
        srcs = [f.read_text(encoding="utf-8") for f in files if f.exists()]
        for fn in called_library_functions(srcs):
            e = look.get(fn.lower())
            if e:
                out.setdefault(e["id"], [])
                if eid not in out[e["id"]]:
                    out[e["id"]].append(eid)
    return out


def reference_with_links():
    entries = load_reference()
    practice = practice_for_entries()
    coding = load_coding()
    qmap = {}
    for s in load_theory().values():
        for q in s["questions"]:
            for r in resolve_refs(q.get("ref")):
                qmap.setdefault(r["id"], []).append({"id": q["id"], "set": s["id"], "prompt": q["prompt"][:110], "type": q["type"]})
    out = []
    for e in entries:
        e = dict(e)
        ids = practice.get(e["id"], [])
        e["practice"] = {"count": len(ids), "exercises": [{"id": i, "title": coding[i]["title"], "stars": coding[i]["stars"]} for i in ids[:6]]}
        qs = qmap.get(e["id"], [])
        e["questions"] = {"count": len(qs), "items": qs[:5]}
        out.append(e)
    return out


# ------------------------------------------------------------------ readiness


def readiness(tag):
    coding, theory, prog = load_coding(), load_theory(), load_progress()
    topics = {}

    def bucket(name):
        return topics.setdefault(name, {"topic": name, "coding_total": 0, "coding_score": 0.0,
                                        "theory_total": 0, "theory_score": 0.0, "missed": []})

    for ex in coding.values():
        if ex["tag"] != tag:
            continue
        b = bucket(ex["topic"])
        b["coding_total"] += 1
        e = prog["coding"].get(ex["id"])
        if e and e.get("passed"):
            b["coding_score"] += 0.5 if e.get("solution_viewed") else 1.0
    missed = []
    for s in theory.values():
        if s["tag"] != tag:
            continue
        for q in s["questions"]:
            b = bucket(q["topic"])
            b["theory_total"] += 1
            e = prog["theory"].get(q["id"])
            if e and e.get("last_correct"):
                b["theory_score"] += 1
            elif e:
                missed.append({"id": q["id"], "set": s["id"], "topic": q["topic"], "prompt": q["prompt"][:140]})
    out = []
    num = den = 0.0
    for b in topics.values():
        parts = []
        if b["coding_total"]:
            parts.append(b["coding_score"] / b["coding_total"])
        if b["theory_total"]:
            parts.append(b["theory_score"] / b["theory_total"])
        b["score"] = sum(parts) / len(parts) if parts else 0.0
        w = b["coding_total"] + b["theory_total"]
        num += b["score"] * w
        den += w
        out.append(b)
    out.sort(key=lambda b: b["score"])
    return {"tag": tag, "overall": (num / den if den else 0.0), "topics": out, "missed": missed}


# ------------------------------------------------------------------ HTTP


class Handler(BaseHTTPRequestHandler):
    server_version = "c-cellar"

    def log_message(self, fmt, *args):
        if os.environ.get("LAB_VERBOSE"):
            super().log_message(fmt, *args)

    def _send(self, code, body, ctype="application/json"):
        if not isinstance(body, (bytes, bytearray)):
            body = json.dumps(body).encode()
        self.send_response(code)
        self.send_header("Content-Type", ctype)
        self.send_header("Content-Length", str(len(body)))
        self.send_header("Cache-Control", "no-store")
        self.end_headers()
        self.wfile.write(body)

    def _body(self):
        n = int(self.headers.get("Content-Length") or 0)
        return json.loads(self.rfile.read(n) or b"{}") if n else {}

    def do_GET(self):
        path = unquote(urlparse(self.path).path)
        try:
            if path.startswith("/api/"):
                return self.api_get(path[5:], urlparse(self.path).query)
            return self.static(path)
        except Exception as e:  # noqa: BLE001 - report to the client instead of dropping the connection
            self._send(500, {"error": f"{type(e).__name__}: {e}"})

    def do_POST(self):
        path = unquote(urlparse(self.path).path)
        try:
            return self.api_post(path[5:], self._body())
        except Exception as e:  # noqa: BLE001
            self._send(500, {"error": f"{type(e).__name__}: {e}"})

    def static(self, path):
        if path in ("", "/"):
            path = "/index.html"
        f = (STATIC / path.lstrip("/")).resolve()
        if STATIC.resolve() not in f.parents or not f.is_file():
            return self._send(404, {"error": "not found"})
        ctype = {".html": "text/html; charset=utf-8", ".js": "text/javascript; charset=utf-8",
                 ".css": "text/css; charset=utf-8", ".svg": "image/svg+xml"}.get(f.suffix, "application/octet-stream")
        self._send(200, f.read_bytes(), ctype)

    # -- GET
    def api_get(self, route, query):
        parts = route.split("/")
        if route == "config":
            return self._send(200, {"vscode_port": VSCODE_PORT, "tags": sorted({p.name for p in CONTENT.iterdir() if p.is_dir()})})
        if route == "index":
            prog = load_progress()
            coding = [{"id": e["id"], "tag": e["tag"], "track": e["track"], "topic": e["topic"], "title": e["title"],
                       "stars": e["stars"], "order": e.get("order", 0), "summary": e.get("summary", ""),
                       "status": coding_status(prog["coding"].get(e["id"]))} for e in load_coding().values()]
            theory = []
            for s in load_theory().values():
                qs = s["questions"]
                st = [prog["theory"].get(q["id"]) for q in qs]
                theory.append({"id": s["id"], "tag": s["tag"], "title": s["title"], "topic": s.get("topic", ""), "chapter": s["chapter"],
                               "count": len(qs), "answered": sum(1 for x in st if x),
                               "correct": sum(1 for x in st if x and x.get("last_correct")),
                               "types": sorted({q["type"] for q in qs}),
                               "qs": [{"d": q.get("difficulty", 1), "a": bool(x), "c": bool(x and x.get("last_correct"))} for q, x in zip(qs, st)]})
            ref = load_reference()
            return self._send(200, {"coding": coding, "theory": theory, "reference": len(ref),
                                    "chapters": load_chapters(), "paths": load_paths()})
        if parts[0] == "coding" and len(parts) == 2:
            ex = load_coding().get(parts[1])
            if not ex:
                return self._send(404, {"error": "unknown exercise"})
            d = Path(ex["dir"])
            ws = ensure_workspace(ex)
            prog = load_progress()["coding"].get(ex["id"], {})
            return self._send(200, {
                "id": ex["id"], "tag": ex["tag"], "title": ex["title"], "track": ex["track"], "topic": ex["topic"],
                "stars": ex["stars"], "statement": (d / "statement.md").read_text(encoding="utf-8"),
                "hints": ex.get("hints", []), "info": ex.get("info", ""), "files": workspace_files(ex),
                "workspace": str(ws), "file": str(ws / workspace_files(ex)[0]), "progress": prog, "status": coding_status(prog)})
        if parts[0] == "theory" and len(parts) == 2:
            s = load_theory().get(parts[1])
            if not s:
                return self._send(404, {"error": "unknown set"})
            prog = load_progress()["theory"]
            return self._send(200, {"id": s["id"], "title": s["title"], "tag": s["tag"], "chapter": s["chapter"],
                                    "practice": chapter_practice(s["tag"], s["chapter"]),
                                    "questions": [dict(public_question(q), progress=prog.get(q["id"])) for q in s["questions"]]})
        if route == "reference":
            return self._send(200, {"entries": reference_with_links()})
        if route == "readiness":
            tag = dict(p.split("=", 1) for p in query.split("&") if "=" in p).get("tag", "t3")
            return self._send(200, readiness(tag))
        return self._send(404, {"error": "not found"})

    # -- POST
    def api_post(self, route, body):
        parts = route.split("/")
        if parts[0] == "coding" and len(parts) == 3:
            ex = load_coding().get(parts[1])
            if not ex:
                return self._send(404, {"error": "unknown exercise"})
            action = parts[2]
            with LOCK:
                prog = load_progress()
                e = prog["coding"].setdefault(ex["id"], {})
                if action == "check":
                    result = check_exercise(ex)
                    e["attempts"] = e.get("attempts", 0) + 1
                    e["last_at"] = int(time.time())
                    e["last_passed_cases"] = sum(1 for c in result["cases"] if c["ok"])
                    e["last_total_cases"] = len(result["cases"])
                    if result["passed"]:
                        e["passed"] = True
                    save_progress(prog)
                    result["status"] = coding_status(e)
                    return self._send(200, result)
                if action == "solution":
                    e["solution_viewed"] = True
                    save_progress(prog)
                    return self._send(200, {"solution": solution_text(ex), "status": coding_status(e)})
                if action == "reset":
                    for n in workspace_files(ex):
                        f = workspace_dir(ex["id"]) / n
                        if f.exists():
                            f.unlink()
                    ensure_workspace(ex)
                    return self._send(200, {"ok": True})
        if parts[0] == "theory" and len(parts) == 3 and parts[2] == "answer":
            s = load_theory().get(parts[1])
            q = next((q for q in s["questions"] if q["id"] == body.get("id")), None) if s else None
            if not q:
                return self._send(404, {"error": "unknown question"})
            correct = grade(q, body.get("response"))
            with LOCK:
                prog = load_progress()
                e = prog["theory"].setdefault(q["id"], {})
                e["attempts"] = e.get("attempts", 0) + 1
                e["correct_count"] = e.get("correct_count", 0) + (1 if correct else 0)
                e["last_correct"] = correct
                e["last_at"] = int(time.time())
                save_progress(prog)
            reply = dict(answer_reveal(q), correct=correct, refs=resolve_refs(q.get("ref")), practice=chapter_practice(s["tag"], s["chapter"]))
            if q["type"] == "fill":
                reply["blanks_ok"] = blanks_ok(q, body.get("response"))
            elif q["type"] in ("match", "sort"):
                reply["items_ok"] = pairs_ok(q, body.get("response"))
            elif q["type"] == "predict" and not correct:
                reply["hints"] = predict_hints(q, body.get("response"))
            return self._send(200, reply)
        if route == "progress/reset":
            with LOCK:
                save_progress({"coding": {}, "theory": {}})
            return self._send(200, {"ok": True})
        return self._send(404, {"error": "not found"})


def main():
    host = os.environ.get("LAB_HOST", "127.0.0.1")
    port = int(os.environ.get("LAB_PORT", "8080"))
    PROGRESS_DIR.mkdir(parents=True, exist_ok=True)
    srv = ThreadingHTTPServer((host, port), Handler)
    print(f"c-cellar is running at http://localhost:{port}", flush=True)
    try:
        srv.serve_forever()
    except KeyboardInterrupt:
        pass


if __name__ == "__main__":
    sys.exit(main())
