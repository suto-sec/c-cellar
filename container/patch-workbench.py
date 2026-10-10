"""Adds frame-keys.js to code-server's page, so VS Code can hand the c-cellar shortcuts to the page around it.

    python3 patch-workbench.py .../out/vs/code/browser/workbench/workbench.html

Run while the image is built. It fails (so the build fails) if the page does not look as expected or the script is not in it afterwards;
a code-server update that changes the page then shows up at once instead of silently losing the shortcuts.
"""
import sys
from pathlib import Path

MARK = "// c-cellar frame keys"
f = Path(sys.argv[1])
html = f.read_text(encoding="utf-8")
script = Path(__file__).with_name("frame-keys.js").read_text(encoding="utf-8")
assert "{{" not in script, "the script must not contain {{ (code-server fills in {{...}} placeholders)"
if MARK not in html:
    assert html.count("<head>") == 1 and "{{WORKBENCH_SCRIPT_NONCE}}" in html, "unexpected workbench.html"
    # (code-server's page only runs scripts that carry its nonce)
    html = html.replace("<head>", '<head>\n\t\t<script nonce="{{WORKBENCH_SCRIPT_NONCE}}">' + MARK + "\n" + script + "</script>", 1)
    f.write_text(html, encoding="utf-8")
assert MARK in f.read_text(encoding="utf-8")
print("patched", f)
