#!/usr/bin/env python3
"""Build the static c-cellar site into site/dist/ from the repo, with no server.

The data is produced by the repo's own loaders (app/server.py), so the site shows exactly what the app shows. Output:
  dist/index.html, app.js (the repo's app.js, patched for the site), style.css, site/*.js (in-browser API, runner, VM glue)
  dist/data/config.json index.json reference.json
  dist/data/coding/<id>.json   one per exercise (statement, hints, solution, tests, files the checker needs)
  dist/data/theory/<set>.json  one per theory set (questions WITH answers: the grading is done in the browser)
Usage: python3 site/build_site.py [--out DIR]
Nothing is installed or downloaded; the VM image (dist/vm/) is made separately by site/build_vm.sh.
"""
import argparse
import base64
import json
import os
import re
import shutil
import sys
import tempfile
from pathlib import Path

HERE = Path(__file__).resolve().parent
REPO = HERE.parent if (HERE.parent / "app" / "server.py").exists() else HERE.parent / "repo"   # (inside the c-cellar repo, or next to it)
sys.path.insert(0, str(REPO / "app"))
os.environ.setdefault("LAB_PROGRESS", tempfile.mkdtemp(prefix="cellar-site-"))
os.environ["LAB_ROOT"] = str(REPO)
import server as S  # noqa: E402


def _memo(fn):
    """The loaders re-read the whole content on every call (the server wants that); a build reads it once."""
    box = []

    def wrapper(*a, **kw):
        if a or kw:
            return fn(*a, **kw)
        if not box:
            box.append(fn())
        return box[0]
    return wrapper


for _name in ("load_coding", "load_theory", "load_reference", "load_chapters", "practice_for_entries", "reference_lookup"):
    setattr(S, _name, _memo(getattr(S, _name)))


def dump(path, data):
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps(data, ensure_ascii=False, separators=(",", ":")), encoding="utf-8")


def text_files(d, pattern):
    return {f.name: f.read_text(encoding="utf-8") for f in sorted(d.glob(pattern)) if f.is_file()}


def fixture_tree(d):
    """{files: {relpath: base64}, dirs: [relpath]} of an exercise's fixtures folder."""
    files, dirs = {}, []
    if d.is_dir():
        for p in sorted(d.rglob("*")):
            rel = p.relative_to(d).as_posix()
            if p.is_dir():
                dirs.append(rel)
            elif p.is_file():
                files[rel] = base64.b64encode(p.read_bytes()).decode("ascii")
    return {"files": files, "dirs": dirs}


def site_compile_cmd(ex):
    """The command typed in the site's terminal: the lab's own build_command without the absolute path of the harness (the machine has it as harness.c)."""
    if ex.get("build"):
        return ex["build"]
    cmd = "gcc -std=gnu17 -Wall -Wextra -g -o prog answer.c"
    if (Path(ex["dir"]) / "harness.c").exists():
        cmd += " harness.c"
    return cmd + " -pthread -lm"


OVERRIDES = json.loads((HERE / "overrides.json").read_text(encoding="utf-8"))


def site_tests(ex, cases):
    """The test cases, with the few site-only overrides of overrides.json applied (the machine is musl, not glibc)."""
    for idx, change in OVERRIDES.get(ex["id"], {}).items():
        cases[int(idx)] = {**cases[int(idx)], **change}
    return cases


UNAVAILABLE = OVERRIDES.get("_unavailable", {})   # {exercise id: why the 32-bit machine cannot grade it}


def site_statement(ex, text):
    why = UNAVAILABLE.get(ex["id"])
    if not why:
        return text
    return text.rstrip() + f"\n\n**Browser edition:** this exercise cannot be checked here. {why} Compile and run still work; check it in the lab version.\n"


def exercise_bundle(ex):
    d = Path(ex["dir"])
    data = {
        "id": ex["id"], "tag": ex["tag"], "title": ex["title"], "track": ex["track"], "topic": ex["topic"], "stars": ex["stars"],
        "minutes": S.minutes_of(ex), "statement": site_statement(ex, (d / "statement.md").read_text(encoding="utf-8")), "site_unavailable": UNAVAILABLE.get(ex["id"]), "hints": ex.get("hints", []),
        "recommended": S.recommended_commands(ex), "date": ex.get("date"), "files": S.workspace_files(ex),
        "compile_cmd": site_compile_cmd(ex), "theory": S.chapter_theory(ex["tag"], ex["topic"]),
        "solution": S.solution_text(ex), "refs": S.exercise_refs(ex),
        # what the in-browser checker needs (the same inputs as app/server.py check_exercise)
        "build": ex.get("build"), "tests": site_tests(ex, json.loads((d / "tests.json").read_text(encoding="utf-8"))["cases"]),
        "headers": text_files(d, "*.h"), "hidden": text_files(d / "hidden", "*") if (d / "hidden").is_dir() else {},
        "harness": (d / "harness.c").read_text(encoding="utf-8") if (d / "harness.c").exists() else None,
        "fixtures": fixture_tree(d / "fixtures"),
    }
    if ex.get("files"):   # multi-file exercise: the reference solution file by file (used to test the checker)
        data["solution_files"] = {n: (d / "solution" / n).read_text(encoding="utf-8") for n in ex["files"]}
    else:
        data["solution_files"] = {"answer.c": (d / "solution.c").read_text(encoding="utf-8")}
    return data


def index_base():
    """The /api/index answer for a student with no progress; the browser lays its own progress over it."""
    coding = [{"id": e["id"], "tag": e["tag"], "track": e["track"], "topic": e["topic"], "title": e["title"], "stars": e["stars"],
               "minutes": e.get("minutes"), "order": e.get("order", 0), "summary": e.get("summary", ""), "status": "todo", "skipped": False}
              for e in S.load_coding().values()]
    theory = []
    for s in S.load_theory().values():
        qs = s["questions"]
        theory.append({"id": s["id"], "tag": s["tag"], "title": s["title"], "topic": s.get("topic", ""), "chapter": s["chapter"], "count": len(qs),
                       "answered": 0, "correct": 0, "types": sorted({q["type"] for q in qs}),
                       "qs": [{"d": q.get("difficulty", 1), "s": S.question_seconds(q), "a": False, "c": False, "id": q["id"]} for q in qs]})
    return {"coding": coding, "theory": theory, "reference": len(S.load_reference()), "chapters": S.load_chapters(),
            "paths": S.load_paths(), "learning_paths": S.load_learning_paths()}


def theory_bundle(s):
    """A theory set with its answers (raw questions), plus what the answer reply carries (references, practice chapter)."""
    qs = []
    for q in s["questions"]:
        raw = dict(q)
        raw["refs"] = S.resolve_refs(q.get("ref"))
        qs.append(raw)
    return {"id": s["id"], "title": s["title"], "tag": s["tag"], "chapter": s["chapter"], "practice": S.chapter_practice(s["tag"], s["chapter"]), "questions": qs}


# The edits the site needs in the repo's app.js. They are applied here, at build time, so the repo's own app.js stays untouched.
# Each is (what to find, what to put). A change that finds nothing stops the build: the app.js changed and this list must follow.
APP_PATCHES = [
    # 1. no server: every API call is answered in the browser
    ('async function api(path, body) {\n  const opt = body === undefined ? {} : { method: "POST", headers: { "Content-Type": "application/json" }, body: JSON.stringify(body) };\n  const r = await fetch("/api/" + path, opt);\n  const j = await r.json();\n  if (r.status === 404 && j.error === "not found") throw new Error("the server is older than this page: restart the lab with ./lab web");\n  if (!r.ok) throw new Error(j.error || r.statusText);\n  return j;\n}',
     'async function api(path, body) {\n  return window.CELLAR_API(path, body);\n}'),
    # 2. the code and terminal panes are in-page panels, not VS Code / ttyd iframes
    ('const frameUrl = (port, query) => `${location.protocol}//${location.hostname}:${port}/?${query}`;\n', ''),
    ('  const payload = encodeURIComponent(JSON.stringify([["openFile", "vscode-remote://" + d.file]]));\n', ''),
    ('const codeFrame = h("iframe", { src: frameUrl(state.cfg.vscode_port, `folder=${encodeURIComponent(d.workspace)}&payload=${payload}`), title: "VS Code", allow: "clipboard-read; clipboard-write" });',
     'const codeFrame = window.CELLAR_SITE.editorPanel(d, h);'),
    ('const termFrame = h("iframe", { src: frameUrl(state.cfg.term_port, "arg=" + encodeURIComponent(d.file.replace(/\\/[^/]*$/, ""))), title: "Terminal", allow: "clipboard-read; clipboard-write" });',
     'const termFrame = window.CELLAR_SITE.terminalPanel(d, h);'),
    ('const termOrigin = `${location.protocol}//${location.hostname}:${state.cfg.term_port}`;', 'const termOrigin = "*";'),
    ('const codeOrigin = `${location.protocol}//${location.hostname}:${state.cfg.vscode_port}`;', 'const codeOrigin = "*";'),
    # Compile / Compile & run: the site compiles in its own VM and shows the output in its terminal panel
    ('    if (!d.compile_cmd) return toast("The server is older than this page: restart the lab with ./lab web");\n    termFrame.contentWindow.postMessage({ cellar: "type", text: `cd "$CELLAR_EX" && ${d.compile_cmd}${more}` }, termOrigin);\n    dock.reveal("term");\n    termFrame.focus();',
     '    dock.reveal("term");\n    window.CELLAR_SITE.compile(d, more.includes("./prog"));'),
    # the daily challenge (and so its streak) needs the Python server: no streak setting on the site
    ('    accent, streakControl(), stepControl(), skipControl(),', '    accent, stepControl(), skipControl(),'),
    ('"The code editor follows your system theme."', '"The editor follows your system theme. Your progress is kept in this browser (Export / Import under the editor)."'),
    # an exercise the site's machine cannot grade (see overrides.json "_unavailable"): say so instead of "it does not compile"
    ('      const nodes = [];\n      if (!r.compiled) {', '      const nodes = [];\n      if (r.unavailable) {\n        nodes.push(h("div", { class: "panel" }, h("b", {}, "This exercise cannot be checked in the browser edition"), h("p", {}, r.unavailable)));\n      } else if (!r.compiled) {'),
    # no stale-frame warning: there are no frames to be stale
    ('const hello = { code: false, term: false };', 'const hello = { code: true, term: true };'),
    ('Write it in ", d.files.map((f, i) => [i ? ", " : "", h("code", {}, f)]), " in VS Code. It saves by itself. "', 'Write it in ", d.files.map((f, i) => [i ? ", " : "", h("code", {}, f)]), " in the editor. It saves by itself in this browser. "'),
]


def patched_app_js():
    src = (REPO / "app" / "static" / "app.js").read_text(encoding="utf-8")
    for old, new in APP_PATCHES:
        if old not in src:
            sys.exit("build_site: this part of app.js is gone, update APP_PATCHES:\n" + old[:160])
        src = src.replace(old, new, 1)
    # words: the site has an editor, not VS Code (the longer phrases first, so that the sentences stay grammatical)
    for old, new in (("in VS Code", "in the editor"), ("inside VS Code", "inside the editor"), ("and VS Code", "and the editor"), ("VS Code and", "the editor and"),
                     ("VS Code over", "the editor over"), ("VS Code first", "Editor first"), ("VS Code;", "the editor;"), ("VS Code (", "the editor ("), ("VS Code", "Editor")):
        src = src.replace(old, new)
    return src


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--out", default=str(HERE / "dist"))
    ap.add_argument("--bundle", action="store_true", help="also write data/bundle.js and load it: the site then works from file:// (used by the browser tests)")
    opts = ap.parse_args()
    out = Path(opts.out)
    if out.exists():
        for child in out.iterdir():
            if child.name != "vm":   # (the VM image is made separately and is big: keep it)
                shutil.rmtree(child) if child.is_dir() else child.unlink()
    out.mkdir(parents=True, exist_ok=True)
    dump(out / "data" / "config.json", {"vscode_port": 0, "term_port": 0, "tags": sorted({e["tag"] for e in S.load_coding().values()})})
    dump(out / "data" / "index.json", index_base())
    for ex in S.load_coding().values():
        dump(out / "data" / "coding" / f"{ex['id']}.json", exercise_bundle(ex))
    for s in S.load_theory().values():
        dump(out / "data" / "theory" / f"{s['id']}.json", theory_bundle(s))
    dump(out / "data" / "reference.json", {"entries": S.reference_with_links()})
    static = REPO / "app" / "static"
    shutil.copy(static / "style.css", out / "style.css")
    (out / "app.js").write_text(patched_app_js(), encoding="utf-8")
    html = (static / "index.html").read_text(encoding="utf-8")
    scripts = "".join(f'<script src="site/{n}"></script>\n' for n in ("tar.js", "runner.js", "api.js", "boot.js", "vm.js", "panels.js"))
    html = html.replace('<link rel="stylesheet" href="style.css">', '<link rel="stylesheet" href="style.css">\n<link rel="stylesheet" href="site/site.css">')
    if opts.bundle:
        data = {f"data/{p.relative_to(out / 'data').as_posix()}": json.loads(p.read_text(encoding="utf-8")) for p in (out / "data").rglob("*.json")}
        (out / "data" / "bundle.js").write_text("window.CELLAR_MOCK_VM=true;window.CELLAR_DATA=" + json.dumps(data, ensure_ascii=False, separators=(",", ":")) + ";", encoding="utf-8")
        scripts = '<script src="data/bundle.js"></script>\n' + scripts
    html = html.replace('<script src="app.js"></script>', scripts + '<script src="app.js"></script>').replace("<title>c-cellar</title>", "<title>c-cellar (browser edition)</title>")
    html = html.replace("</main>", '</main>\n<footer id="site-footer">Browser edition: your C is compiled by gcc in a small Linux computer running inside this page (the v86 emulator and Alpine Linux, free software: <a href="vm/NOTICE.txt">licences and sources</a>). Your progress stays in this browser.</footer>', 1)
    (out / "index.html").write_text(html, encoding="utf-8")
    (out / ".nojekyll").write_text("", encoding="utf-8")
    shutil.copytree(HERE / "src", out / "site", dirs_exist_ok=True)
    n = sum(1 for _ in (out / "data").rglob("*.json"))
    size = sum(f.stat().st_size for f in out.rglob("*") if f.is_file())
    print(f"site built in {out}: {n} data files, {size / 1e6:.1f} MB")


if __name__ == "__main__":
    main()
