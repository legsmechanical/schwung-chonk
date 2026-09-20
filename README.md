# CHONK — a Schwung module

A physically modeled electric bass monosynth for Schwung on the Ableton Move,
ported from [**OneTrick CHONK**](https://punklabs.com/ot-chonk) by Punk Labs LLC
(GPL-3.0-or-later). *It is free as in rights, not as in beer — if you play this,
[buy a copy](https://punklabs.com/ot-chonk).*

One waveguide, one string: a bass string with bridge and nut terminations, a
pickup tapped part-way along it, a finger pluck and a noise transient at the
strike. Not a sample, not a subtractive patch pretending — the reason a hard
note rings differently from a soft one is that the model is being hit harder.

## What was ported, and how

The synth **is** Punk Labs' Faust code. `src/dsp/faust/bass.dsp` (the string)
and `output.dsp` (EQ → saturation → panner) are their files, with `onetrick.lib`
and `params.lib` beside them. They carry exactly one local change — the Ring
release control, marked `SCHWUNG PORT ADDITION` at both sites and described
below; everything else is upstream byte for byte. `scripts/gen_dsp.sh` compiles them
to C++ — upstream's own build line with `-lang cpp` in place of `-lang rust` —
into `src/dsp/generated/`, which is machine output and never hand-edited.

What this repo replaces is upstream's nih-plug/egui host:
`src/dsp/chonk_plugin.cpp` is the `plugin_api_v2` wrapper holding the voice
logic, the MIDI surface and the parameter surface Schwung reads. The voice
semantics are transcribed from upstream's `OneTrickMonoVoice` — including the
edge cases, which are load-bearing (see the comments in the wrapper).

The original source drop is kept under `original source/` for provenance.

## Playing it

**Range: C3–G6 (MIDI 48–91).** Below that the waveguide runs out of delay line,
above it the model goes unstable, so upstream clamps it and so do we. Notes
outside the window are ignored rather than folded in.

**Articulations** work two ways — a pad while you hold it, a parameter the rest
of the time:

| | keyswitch | parameter | how the two combine |
|---|---|---|---|
| Style: Finger / Pick / Slap | D2 / E2 / F2 | `style` (enum) | the held pad wins; releasing it returns to the enum |
| Mute | C2 | `mute` | on if **either** says so |
| Legato | F#2, or CC 68 | `legato` | on if either says so |
| Ring / Gate | G2 | `let_ring` | on if either says so |
| Slide down | C#2 | — | momentary only |
| Slide up | D#2 | — | momentary only |

**Finger, Pick and Slap are one choice, not three switches.** `params.lib` folds
them with `max()` — `articulationStyleAmount = max(finger, pick, slap)` — so
holding two just means the harder one wins and the other is inaudible. Three
toggles would have offered a state the model cannot represent, so they are one
enum.

The keyswitch octave moves with the module's octave shift, because on Move these
are pads on the same grid and a fixed C2 row would walk off the bottom of it.
Slide is velocity-scaled and ramps for as long as the key is held, which is why
it has no latch: a latched slide would climb 48 semitones and stay there.

Slap also arrives on its own above velocity ~0.85 (upstream's
`articulationSlapAuto`) — hit the pads hard and it is there without a keyswitch.

### Two upstream behaviours worth knowing, because they read as broken knobs

**Strike Hardness is bypassed whenever a style is engaged**, and again above
velocity ~108. `bass.dsp:113` is
`interpolate_linear(articulationStyleAmount, strikeHardness, articulationStyleHardness)`:
the knob is only in circuit while `articulationStyleAmount` is 0. Since
`articulationSlapAuto` ramps in from velocity 0.85, a hard-played note bypasses
it too — measured, a 0 → 100 sweep at velocity 127 changes the output by
nothing at all (rms 0.08413 either way). At velocity 100 it moves peak
0.41 → 0.34. This is upstream's design, not a porting fault.

**Thump is disabled by Mute and by Legato.** `extraThump` is pinned to 1.5 when
Mute is on, and the whole term is multiplied by `(1 - articulationLegato)`.
With neither engaged the knob is strong: attack rms 0.048 → 0.205 across its
range. `./scripts/render.sh` renders both of these as A/B pairs.

**Mono, by design.** A new note takes the string over; releasing it hands the
string back to the note below without re-plucking. That is the legato behaviour,
not a limitation to be fixed with more voices.

Mod wheel opens the string's sustain right up. Aftertouch bends (range set by
`at_range`), pitch bend by `pw_range`. Sustain pedal (CC 64) holds notes — the
wrapper's addition; upstream's DSP has no use for it.

## Parameters

| Page | Params |
|---|---|
| root | Pickup Pos, Brightness, Strike Hard, Thump, Tone, Sustain, Ring, Volume + preset browser |
| String | the six above, plus Ring |
| Articulation | Style (Off/Finger/Pick/Slap), Mute, Legato, Ring/Gate |
| EQ | 100 Hz shelf, 250 / 500 / 1.5 k peaks, 3 kHz shelf (±12 dB) |
| Mix | Volume, Pan, Saturation |
| MIDI | Fine Tune, Bend Range, AT Range, Vel Sens |

Volume is `-60…+6 dB`, a narrowed slice of upstream's `-100…+6`: the bottom
40 dB of that range are all silence and are not worth a knob sweep. Everything
else keeps upstream's range exactly.

### Ring — the one thing added to the DSP

Upstream damps the string hard on note-off (`bounceEfficiencyMuted = 0.5`) and
offers no control over it. **Ring** (String page, 0–100%) sets how much the
release damps: 0 is upstream's behaviour to the sample, 100 leaves the string
undamped so it decays at its own sustain. The interpolation is done on the
**log** of the two bounce efficiencies, because 0.5 and 0.999 are three orders
of magnitude apart in decay time and a linear knob would do nothing for its
first 80%.

**Ring/Gate** (`let_ring`, G2) overrides the knob while engaged — a let-ring
gesture, the way a sustain pedal is, rather than another value to dial in.

Both are marked `SCHWUNG PORT ADDITION` in `params.lib` and `bass.dsp`; they are
the only edits to Punk Labs' DSP sources.

**9 factory presets** ship compiled in, converted from Punk Labs' `.preset`
files: J-Bass, Defender, Demonster, Disco, Flatworm, Gargantuan, Justice,
New Strings, Sitcom.

## Building

```sh
./scripts/build.sh      # cross-compiles for Move via Docker -> dist/chonk-module.tar.gz
./scripts/install.sh    # deploys to ableton@move.local (MOVE_HOST to override)
./scripts/test.sh       # host-side tests, no device needed
./scripts/render.sh     # audition WAV -> build/chonk_demo.wav
./scripts/gen_dsp.sh    # regenerate the C++ from the .dsp sources (needs `brew install faust`)
```

Faust is needed only to regenerate the DSP; the build container has a cross
compiler and nothing else.

Two files are generated from the wrapper and must be regenerated when
parameters change (`scripts/build.sh` does it): `src/module.json`'s
`ui_hierarchy`, and `src/dsp/factory_bank.h`, which is indexed by `PARAMS[]`
**position** — insert a parameter without regenerating and every preset value
past the insertion loads one slot out, silently.

## License

GPL-3.0-or-later, inherited from OneTrick CHONK. `LICENSE` is upstream's copy;
`original source/OneTrickCHONK-Source-v1.0.0/NOTICES.txt` carries the full
third-party acknowledgments (Faust's libraries, nih-plug, and the rest).

    OneTrick CHONK, a physically modeled electric bass monosynth.
    Copyright (C) 2024 Punk Labs LLC
