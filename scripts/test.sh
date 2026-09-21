#!/usr/bin/env bash
# Build and run the host-side tests natively (no device, no Docker).
set -e
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
REPO_ROOT="$(dirname "$SCRIPT_DIR")"
cd "$REPO_ROOT"
mkdir -p build
CXX_BIN="${CXX:-c++}"
"$CXX_BIN" -std=c++14 -O2 -Wall -Wno-comment -Isrc/dsp \
    tests/chonk_test.cpp -o build/chonk_test -lm
./build/chonk_test

# help.json is checked here rather than at runtime: the device simply runs a
# long line off the 128x64 screen, it does not complain.
if command -v node >/dev/null 2>&1; then
    node tests/hierarchy.test.mjs
    node tests/help.test.mjs
else
    echo "WARNING: node not found - help.json NOT linted"
fi
