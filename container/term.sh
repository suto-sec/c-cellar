#!/bin/bash
# The terminal pane: ttyd runs this with the folder of the exercise that is open (from the ?arg= of its URL).
dir="$(realpath -m -- "${1:-/lab/.progress/workspace}")"
case "$dir" in /lab/.progress/workspace|/lab/.progress/workspace/*) ;; *) dir=/lab/.progress/workspace ;; esac
[ -d "$dir" ] || dir=/lab/.progress/workspace
cd "$dir" || exit 1
exec bash --rcfile /lab/container/term.rc -i
