#!/usr/bin/env bash
# Render an audition WAV of the module (build/chonk_demo.wav) so a change can
# be heard. Native build, no device needed.
set -e
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
REPO_ROOT="$(dirname "$SCRIPT_DIR")"
cd "$REPO_ROOT"
mkdir -p build
"${CXX:-c++}" -std=c++14 -O2 -Wno-comment -Isrc/dsp \
    tests/render_wav.cpp -o build/chonk_render -lm
exec ./build/chonk_render "${1:-build/chonk_demo.wav}"
