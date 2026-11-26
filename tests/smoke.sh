#!/bin/sh
# Headless runs of the real binary.
#  1. stdin is a pipe: after the two numbers it reaches EOF, getch() returns ERR
#     every tick, nobody fires, and the snake must reach the cannon -> "Game Over".
#  2. through a pseudo-terminal: press 'q' one second in. A 20x20 game would last
#     ~320 s on its own, so finishing inside the timeout proves the key quits.
set -eu
BIN=${1:-./snake}

out=$(printf '6 3\n' | TERM=xterm timeout 30 "$BIN")
echo "$out" | grep -q 'Game Over' || { echo "FAIL: game did not end on its own"; exit 1; }
echo "ok: snake reaches the cannon and the game ends"

out=$( (printf '20 8\n'; sleep 1; printf q; sleep 1; printf x) | TERM=xterm timeout 20 script -qec "$BIN" /dev/null) \
  || { echo "FAIL: 'q' did not quit (exit $?)"; exit 1; }
echo "$out" | grep -q 'Game Over' || { echo "FAIL: no Game Over after 'q'"; exit 1; }
echo "ok: 'q' quits"
