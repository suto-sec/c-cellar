"""Writes ttyd's own page with frame-keys.js and term-bridge.js added: the terminal then hands the c-cellar shortcuts to the page around it
and types the lines that page asks for (the Compile buttons).

    python3 ttyd-index.py OUT.html

ttyd has no hook for this, but it can serve a replacement page (--index): this one is ttyd's stock page, fetched from a throw-away
ttyd, plus one <script>. Exit status 1 (and no file) when anything goes wrong; the caller then starts plain ttyd.
"""
import gzip
import json
import os
import subprocess
import sys
import time
import urllib.request
from pathlib import Path

PORT = 18082
HERE = Path(__file__).parent


def scripts():
    """frame-keys.js (the shortcuts) and term-bridge.js (lines typed by the c-cellar page), which may only come from that page's origin."""
    port = os.environ.get("LAB_HOST_PORT", "8080")
    origins = json.dumps([f"http://localhost:{port}", f"http://127.0.0.1:{port}"])
    bridge = (HERE / "term-bridge.js").read_text(encoding="utf-8").replace("__ORIGINS__", origins)
    return (HERE / "frame-keys.js").read_text(encoding="utf-8") + "\n" + bridge


def stock_page():
    p = subprocess.Popen(["ttyd", "--port", str(PORT), "--interface", "lo", "true"], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    try:
        for _ in range(50):
            try:
                with urllib.request.urlopen(f"http://127.0.0.1:{PORT}/", timeout=2) as r:
                    data = r.read()
                    return (gzip.decompress(data) if r.headers.get("Content-Encoding") == "gzip" else data).decode("utf-8")
            except OSError:
                time.sleep(0.1)
        raise RuntimeError("ttyd did not answer")
    finally:
        p.terminate()
        p.wait()


def main():
    page = stock_page()
    if "<head>" not in page or len(page) < 10000:
        raise RuntimeError("unexpected ttyd page")
    Path(sys.argv[1]).write_text(page.replace("<head>", "<head><script>" + scripts() + "</script>", 1), encoding="utf-8")


if __name__ == "__main__":
    try:
        main()
    except Exception as e:  # noqa: BLE001
        print("ttyd-index:", e, file=sys.stderr)
        sys.exit(1)
