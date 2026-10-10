"""Writes ttyd's own page with frame-keys.js added, so the terminal can hand the c-cellar shortcuts to the page around it.

    python3 ttyd-index.py OUT.html

ttyd has no hook for this, but it can serve a replacement page (--index): this one is ttyd's stock page, fetched from a throw-away
ttyd, plus one <script>. Exit status 1 (and no file) when anything goes wrong; the caller then starts plain ttyd.
"""
import gzip
import subprocess
import sys
import time
import urllib.request
from pathlib import Path

PORT = 18082
SCRIPT = Path(__file__).with_name("frame-keys.js").read_text(encoding="utf-8")


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
    Path(sys.argv[1]).write_text(page.replace("<head>", "<head><script>" + SCRIPT + "</script>", 1), encoding="utf-8")


if __name__ == "__main__":
    try:
        main()
    except Exception as e:  # noqa: BLE001
        print("ttyd-index:", e, file=sys.stderr)
        sys.exit(1)
