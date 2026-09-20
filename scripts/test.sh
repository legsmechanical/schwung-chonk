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
exec ./build/chonk_test
