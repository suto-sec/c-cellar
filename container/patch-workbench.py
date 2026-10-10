"""Adds frame-keys.js and vscode-init.js to code-server's page: VS Code then hands the c-cellar shortcuts to the page around it and starts
with its explorer closed.

    python3 patch-workbench.py .../out/vs/code/browser/workbench/workbench.html

Run while the image is built. It fails (so the build fails) if the page does not look as expected or the scripts are not in it afterwards;
a code-server update that changes the page then shows up at once instead of silently losing the shortcuts. Running it again replaces
what an earlier run added.
"""
import re
import sys
from pathlib import Path

START, END = "// c-cellar start", "// c-cellar end"
f = Path(sys.argv[1])
html = f.read_text(encoding="utf-8")
here = Path(__file__).parent
script = "\n".join((here / n).read_text(encoding="utf-8") for n in ("frame-keys.js", "vscode-init.js"))
assert "{{" not in script, "the scripts must not contain {{ (code-server fills in {{...}} placeholders)"
# (code-server's page only runs scripts that carry its nonce)
block = '<script nonce="{{WORKBENCH_SCRIPT_NONCE}}">' + START + "\n" + script + "\n" + END + "</script>"
if START in html:
    html = re.sub(r"<script nonce=\"\{\{WORKBENCH_SCRIPT_NONCE\}\}\">" + re.escape(START) + r".*?" + re.escape(END) + "</script>", lambda m: block, html, flags=re.S)
else:
    assert html.count("<head>") == 1 and "{{WORKBENCH_SCRIPT_NONCE}}" in html, "unexpected workbench.html"
    html = html.replace("<head>", "<head>\n\t\t" + block, 1)
f.write_text(html, encoding="utf-8")
assert START in f.read_text(encoding="utf-8") and "cellar.explorer.closed" in f.read_text(encoding="utf-8")
print("patched", f)
