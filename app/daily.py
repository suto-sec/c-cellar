"""Daily exercise: every day the app builds one new exercise from a template of the course topic.

A template (content/<tag>/daily.py) is a function that gets a random generator and returns a *spec*: the statement, the hints, a reference
solution and the test cases. The expected output of every test is computed by the template itself in Python, never by running the solution.
The spec is written once, as an ordinary exercise folder in .progress/daily/<id>/, so the checker, the workspace and the answer work as for
any other exercise. Standard library only.
"""
import importlib.util
import json
import os
import random
import re
import shutil
import tempfile
from datetime import date, timedelta
from pathlib import Path

ID_RE = re.compile(r"^([a-z][a-z0-9]*)-daily-(\d{4})(\d{2})(\d{2})$")
RECENT = 5   # a template is not repeated within this many days


def valid_date(text):
    """The date as a date object if it is a real YYYY-MM-DD, else None."""
    if not isinstance(text, str) or not re.fullmatch(r"\d{4}-\d{2}-\d{2}", text):
        return None
    try:
        return date.fromisoformat(text)
    except ValueError:
        return None


def daily_id(tag, day):
    return f"{tag}-daily-{day.strftime('%Y%m%d')}"


def id_date(ex_id):
    """(tag, date) of a daily exercise id, or None if it is not one."""
    m = ID_RE.match(str(ex_id))
    if not m:
        return None
    try:
        return m.group(1), date(int(m.group(2)), int(m.group(3)), int(m.group(4)))
    except ValueError:
        return None


def load_templates(content):
    """[{id, tag, chapter, fn}] from every content/<tag>/daily.py (a list TEMPLATES of (id, chapter, function))."""
    out = []
    for f in sorted(Path(content).glob("*/daily.py")):
        spec = importlib.util.spec_from_file_location("daily_" + f.parent.name, f)
        mod = importlib.util.module_from_spec(spec)
        assert spec.loader is not None
        spec.loader.exec_module(mod)
        for tid, chapter, fn in mod.TEMPLATES:
            out.append({"id": tid, "tag": f.parent.name, "chapter": chapter, "fn": fn})
    return out


# ------------------------------------------------------------------ writing a spec as an exercise folder


def example_text(case):
    """The Example of a statement, from the first test case (standard input or arguments)."""
    out = case.get("stdout", "")
    if "stdin" in case:
        return f"Input:\n```\n{case['stdin'].rstrip(chr(10))}\n```\nOutput:\n```\n{out.rstrip(chr(10))}\n```\n"
    if "args" in case:
        shown = " ".join(["./prog"] + [a if re.fullmatch(r"[\w./-]+", a) else '"' + a + '"' for a in case["args"]])
        return f"```\n$ {shown}\n{out.rstrip(chr(10))}\n```\n"
    return ""


def write_exercise(d, ex_id, tag, chapter, template_id, day, spec):
    """Write the spec into folder d (created); meta.json goes last, so a folder with a meta is always complete."""
    d = Path(d)
    d.mkdir(parents=True)
    statement = f"# {spec['title']}\n\n{spec['statement'].strip()}\n"
    example = spec.get("example")
    if example is None and spec["tests"]:
        example = example_text(spec["tests"][0])
    if example:
        statement += "\n**Example**\n\n" + example
    (d / "statement.md").write_text(statement, encoding="utf-8")
    (d / "tests.json").write_text(json.dumps({"cases": spec["tests"]}, indent=1) + "\n", encoding="utf-8")
    if spec.get("solution_files"):
        (d / "solution").mkdir()
        for n, text in spec["solution_files"].items():
            (d / "solution" / n).write_text(text, encoding="utf-8")
    else:
        (d / "solution.c").write_text(spec["solution"], encoding="utf-8")
    if spec.get("harness"):
        (d / "harness.c").write_text(spec["harness"], encoding="utf-8")
    for n, text in spec.get("headers", {}).items():
        (d / n).write_text(text, encoding="utf-8")
    if spec.get("hidden"):
        (d / "hidden").mkdir()
        for n, text in spec["hidden"].items():
            (d / "hidden" / n).write_text(text, encoding="utf-8")
    meta = {"id": ex_id, "tag": tag, "track": "daily", "topic": chapter, "title": spec["title"], "stars": spec["stars"],
            "summary": spec["summary"], "hints": spec["hints"], "template": template_id, "date": day.isoformat()}
    if spec.get("files"):
        meta["files"] = spec["files"]
    if spec.get("build"):
        meta["build"] = spec["build"]
    (d / "meta.json").write_text(json.dumps(meta, indent=1, ensure_ascii=False) + "\n", encoding="utf-8")


def generate(template, seed):
    """The spec of a template for a seed (the same seed always gives the same spec)."""
    return template["fn"](random.Random(seed))


def load(d):
    meta = json.loads((Path(d) / "meta.json").read_text(encoding="utf-8"))
    meta["dir"] = str(d)
    return meta


def recent_templates(root, day, tag):
    """Template ids of the exercises of the RECENT days before `day`."""
    seen = []
    for k in range(1, RECENT + 1):
        d = Path(root) / daily_id(tag, day - timedelta(days=k))
        if (d / "meta.json").exists():
            seen.append(load(d).get("template"))
    return seen


def parse_levels(text):
    """The star levels of a comma separated list ("2,3") as a set of ints from 1 to 5; empty means all levels (None)."""
    levels = {int(x) for x in str(text or "").split(",") if x.strip().isdigit() and 1 <= int(x) <= 5}
    return levels or None


def available_stars(templates, tag):
    """The star levels the templates of a topic can produce (a few specs of each are built to see)."""
    return sorted({generate(t, f"stars-{k}")["stars"] for t in templates if t["tag"] == tag for k in range(6)})


def choose(root, templates, tag, day, levels=None):
    """(template, spec) of a day. The template comes from the date, never one of the last RECENT days; with `levels` the first one
    (in that same date-seeded order) whose exercise has one of those star levels, else the closest level."""
    mine = [t for t in templates if t["tag"] == tag]
    recent = recent_templates(root, day, tag)
    pool = [t for t in mine if t["id"] not in recent] or mine
    rng = random.Random("pick-" + day.isoformat())
    first = rng.choice(pool)
    others = [t for t in pool if t is not first]
    rng.shuffle(others)
    outside = [t for t in mine if t not in pool]
    rng.shuffle(outside)
    seed = "spec-" + day.isoformat()
    best = None
    for template in [first] + others + outside:
        spec = generate(template, seed)
        if not levels or spec["stars"] in levels:
            return template, spec
        gap = min(abs(spec["stars"] - n) for n in levels)
        if best is None or gap < best[0]:
            best = (gap, template, spec)
    return best[1], best[2]


def for_date(root, templates, tag, day, levels=None):
    """The exercise of a day, generated and written the first time it is asked for. `root` is the folder of daily exercises."""
    ex_id = daily_id(tag, day)
    d = Path(root) / ex_id
    if (d / "meta.json").exists():
        return load(d)
    if not any(t["tag"] == tag for t in templates):
        return None
    template, spec = choose(root, templates, tag, day, levels)
    Path(root).mkdir(parents=True, exist_ok=True)
    tmp = Path(tempfile.mkdtemp(prefix=ex_id + ".", dir=root))
    try:
        write_exercise(tmp / "x", ex_id, tag, template["chapter"], template["id"], day, spec)
        try:
            os.rename(tmp / "x", d)   # two requests may race on the first load of the day: the second rename fails and uses the first folder
        except OSError:
            if not (d / "meta.json").exists():
                raise
    finally:
        shutil.rmtree(tmp, ignore_errors=True)
    return load(d)


def find(root, ex_id):
    """An existing daily exercise by id (never generates one), or None."""
    parsed = id_date(ex_id)
    d = Path(root) / str(ex_id)
    if not parsed or not (d / "meta.json").exists():
        return None
    return load(d)


# ------------------------------------------------------------------ the streak


def streak(days, today):
    """days: set of 'YYYY-MM-DD' on which the daily exercise was passed that same day; today: 'YYYY-MM-DD'.
    current = consecutive days up to today (or up to yesterday while today is still open); best = the longest run ever."""
    t = valid_date(today)
    have = {d for d in (valid_date(x) for x in days) if d}
    if t is None:
        return {"current": 0, "best": 0, "today_done": False}
    cur = 0
    k = t if t in have else t - timedelta(days=1)
    while k in have:
        cur += 1
        k -= timedelta(days=1)
    best = run = 0
    prev = None
    for d in sorted(have):
        run = run + 1 if prev and d - prev == timedelta(days=1) else 1
        best = max(best, run)
        prev = d
    return {"current": cur, "best": best, "today_done": t in have}


def history(root, before):
    """Metas of the daily exercises that exist for days before `before` (a date), newest first."""
    out = []
    for d in Path(root).glob("*-daily-*") if Path(root).is_dir() else []:
        parsed = id_date(d.name)
        if parsed and parsed[1] < before and (d / "meta.json").exists():
            out.append(load(d))
    return sorted(out, key=lambda m: m["date"], reverse=True)
