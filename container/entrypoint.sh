#!/bin/bash
# Starts a terminal (ttyd, port 8082), VS Code (code-server, port 8081) and the c-cellar app (port 8080). The project is mounted at /lab.
set -e
export HOME="${HOME:-/lab/.progress/home}"
[ -w "$HOME" ] 2>/dev/null || export HOME=/lab/.progress/home
mkdir -p "$HOME" /lab/.progress/workspace
export LAB_ROOT=/lab LAB_HOST=0.0.0.0 LAB_PORT=8080

if [ "$1" != "" ]; then exec "$@"; fi   # `./lab shell` and `./lab selftest` pass their own command

SETTINGS="$HOME/.local/share/code-server/User/settings.json"
if [ ! -f "$SETTINGS" ]; then
  mkdir -p "$(dirname "$SETTINGS")"
  cat > "$SETTINGS" <<'JSON'
{
  "files.autoSave": "afterDelay",
  "files.autoSaveDelay": 400,
  "workbench.startupEditor": "none",
  "workbench.tips.enabled": false,
  "chat.disableAIFeatures": true,
  "chat.commandCenter.enabled": false,
  "workbench.secondarySideBar.defaultVisibility": "hidden",
  "workbench.layoutControl.enabled": false,
  "breadcrumbs.enabled": false,
  "workbench.colorTheme": "Default Dark Modern",
  "window.autoDetectColorScheme": true,
  "workbench.preferredLightColorTheme": "Default Light Modern",
  "workbench.preferredDarkColorTheme": "Default Dark Modern",
  "security.workspace.trust.enabled": false,
  "telemetry.telemetryLevel": "off",
  "editor.minimap.enabled": false,
  "editor.fontSize": 15,
  "editor.tabSize": 4,
  "editor.insertSpaces": true,
  "terminal.integrated.defaultProfile.linux": "bash",
  "update.mode": "none",
  "extensions.autoCheckUpdates": false
}
JSON
fi

code-server --bind-addr 0.0.0.0:8081 --auth none --disable-telemetry --disable-update-check \
  --user-data-dir "$HOME/.local/share/code-server" --extensions-dir "$HOME/.local/share/code-server/extensions" \
  /lab/.progress/workspace >/lab/.progress/code-server.log 2>&1 &

# One bash per open terminal pane, started in the exercise folder it asks for (see term.sh). --check-origin keeps other websites out.
ttyd --port 8082 --interface 0.0.0.0 --writable --url-arg --check-origin -t fontSize=15 -t cursorBlink=true \
  /lab/container/term.sh >/lab/.progress/ttyd.log 2>&1 &

exec python3 /lab/app/server.py
