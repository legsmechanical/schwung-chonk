#!/usr/bin/env bash
# Regenerate the Faust DSP as C++ from Punk Labs' .dsp sources.
#
# The synth IS the Faust code (src/dsp/faust/*.dsp + onetrick.lib): everything
# in src/dsp/generated/ is machine output and must never be hand-edited. Run
# this after touching any .dsp/.lib, then rebuild. Requires `faust`
# (brew install faust). The flags mirror OneTrick's own README build line, with
# -lang cpp in place of -lang rust: -ct 0 (no table check) and --timeout 0 are
# load-bearing for this DSP's big wavetables.
set -e
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
REPO_ROOT="$(dirname "$SCRIPT_DIR")"
cd "$REPO_ROOT/src/dsp"

command -v faust >/dev/null 2>&1 || { echo "error: faust not found (brew install faust)"; exit 1; }
echo "Faust: $(faust --version | head -1)"

gen() { # <dsp file> <class name> <output header>
    echo "  $1 -> generated/$3 ($2)"
    faust --check-table 0 --timeout 0 --process-name process -lang cpp \
        --import-dir faust -cn "$2" -dlt 65536 \
        -o "generated/$3" "faust/$1"
}

gen bass.dsp   DSP_Bass   dsp_bass.hpp
gen output.dsp DSP_Output dsp_output.hpp
echo "Done."
