# CHONK

**A physically modeled electric bass for Schwung on the Ableton Move.**

Ported from [**OneTrick CHONK**](https://punklabs.com/ot-chonk) by **Punk Labs
LLC** — the bass model is entirely theirs. *It is free as in rights, not as in
beer: if you play this, [buy a copy](https://punklabs.com/ot-chonk).*

One modelled string: bridge and nut terminations, a pickup tapped part-way
along it, a finger pluck and a noise transient at the strike. A hard note rings
differently from a soft one because the string is being hit harder — not
because a different sample was chosen.

---

## Manual

### Playing

**Range C3–G6 (MIDI 48–91).** Notes outside it are ignored rather than folded
in: below, the waveguide runs out of delay line; above, the model goes
unstable. Upstream clamps the same window.

**Monophonic by design.** A new note takes the string over; releasing it hands
the string back to the note below without re-plucking. That handover is how one
string works — it is not the Legato switch, which is off by default.

Play hard (velocity 108 and up) and it slaps on its own — upstream's
`articulationSlapAuto`.

### The pads

The octave below the playing range is articulation, and moves with the module's
octave shift so the row stays reachable.

| Pad | | Pad | |
|---|---|---|---|
| **C2** | Mute | **E2** | Pick |
| **C#2** | Slide down | **F2** | Slap |
| **D2** | Finger | **F#2** | Legato |
| **D#2** | Slide up | **G2** | Ring |
| | | **G#2** | Alt Pick |

**A held pad always wins, and never moves the parameter.** Style pads select
that style while held, then hand back to the Style setting. Mute and Legato
pads *invert* their switch — with Mute latched on, hold the pad to play one
open note. Slide pads bend for as long as they are held, scaled by velocity.

### Knobs and pages

The front page is the string, and the sub-pages split **by hand** — what the
striking hand is doing, and what happens between notes.

```
root  ·  the string                        + preset browser
  1 Pickup   2 Bright   3 Tone             timbre
  4 Thump                                  attack body
  5 Sustain  6 Ring                        how long it holds, how it lets go
  7 Sat      8 Volume                      output

  Attack     Style · Strike · Mute · Alt Pick · Pick Depth
  Fretting   Legato · Glide · Glide Time · Frets · Retrigger
  Output     Volume · Pan · Sat · EQ 100 · 250 · 500 · 1.5k · 3k
  MIDI       Fine Tune · Bend Range · AT Range · Vel Sens
```

**Style and Strike Hardness share a page because they are one control** — a
style does nothing but replace Strike with a fixed value. Split across pages,
as they were, the knob reads as broken. Saturation and Volume are on the root
knob row but live on Output; a level's knobs need not be in its own params, so
the row is what you play while the menu stays the string and four doors.

**Pickup Pos** moves the pickup along the string — low is fat and neck-ish,
high is nasal and bridge-ish. **Tone** is the passive tone control, a 1-pole
lowpass from 300 to 2500 Hz. **Volume** runs −60…+6 dB, a narrowed slice of
upstream's −100…+6 (the bottom 40 dB are all silence).

### Attack and Fretting

**Style — Off / Finger / Pick / Slap.** One choice, not four switches: in the
model a style does nothing but replace the Strike Hardness knob with a set
value — Finger 0, Pick 50, Slap 100, Off *the knob*. So **Off with Strike
Hardness at 100 is Slap**, bit for bit.

**Mute** damps the string fast and dull and gives the attack its own extra
thump. While it is on the Thump knob is out of circuit.

**Legato** is a sound, not a note-handling mode: it glides the pitch over 35 ms
stepping through frets, and takes the click and the finger out of the output.
The note-to-note handover happens with Legato off as well.

**Glide**, **Glide Time** and **Frets** are the first two of those broken out.
Glide slides between held notes while *keeping* the click, over **Glide Time**
— 0–500 ms, higher is slower, defaulting to upstream's fixed 35 ms; Legato still implies
glide, so Legato alone sounds exactly as it always has. Frets sets the path a
glide takes — on, it walks up the frets; off, it slides smoothly past them —
and defaults on, which is how upstream always played. All three only matter
when one note is held into the next; with Glide and Legato both off, Glide Time
does nothing at all, because the DSP multiplies it by `glideTerm`.

**Alt Pick** plays the line down, up, down, up instead of every note being a
downstroke. Three things separate the two strokes: the pick pushes the string
the other way (the part you hear against a string that is still ringing), the
upstroke carries about 18% less energy, and its click is thinner, brighter and
shorter. A phrase starts on a downstroke, and the count restarts after a **rest** of
~1 s — time with nothing sounding. Not on release (picking is separate notes,
so that would make every note a downstroke), and not on a gap between attacks
either: measured that way, a quarter note at 120 bpm is a 500 ms gap, so
anything slower than eighths came out all downstrokes and a held note reset
itself mid-ring. Pad G#2.

**Pick Depth** scales that separation: 100% is the voicing tuned by ear, 0%
leaves only the direction flip, 200% doubles it. It deliberately does not touch
the *direction* — that part is physically true and it is binary, and scaling it
would put the upstroke's excitation at zero around 50% and drop every other
note. What scales is the three magnitudes, which were the judgement calls.

**Retrigger** decides what an overlapping note does. Off (the original): a
second note at the *same* velocity re-pitches the ringing string without
re-plucking; at a different velocity it plucks. On: every note-on plucks.

**Ring** sets how far the release damps the string — 0 is the original's hard
stop, 100 leaves it undamped to decay on its own. The G2 pad rings the string
out while held, whatever the knob says.

### MIDI

Pitch bend by **Bend Range** (0–12 st). Aftertouch bends by **AT Range**
(−2…+2 st; negative bends down). **Fine Tune** detunes ±100 cents. **Vel Sens**
sets how far velocity moves the sound — at 0 every note plays as velocity 80.

CC 1 mod wheel opens the sustain all the way up · CC 64 sustain holds notes ·
CC 68 legato · CC 120/123 all notes off.

### Presets

Nine, from Punk Labs: **J-Bass, Defender, Demonster, Disco, Flatworm,
Gargantuan, Justice, New Strings, Sitcom.** They set the String, EQ, Mix and
MIDI pages, and reset the switches this port added — Glide, Glide Time, Frets,
Ring, Alt Pick, Retrigger — because those are part of the sound. Upstream's own
articulation (Style, Mute, Legato) is left alone, because that is how you are
playing rather than what the patch is.

On-device help covers all of the above (Shift+Vol+Menu → Help → Modules).

---

## Behaviours that read as broken knobs

Both are upstream's design, and both were measured rather than assumed:

**Strike Hardness is subtle, and often bypassed entirely.** `bass.dsp:122`
interpolates *away* from the knob as `articulationStyleAmount` rises, so any
engaged Style takes it out of circuit — and since `articulationSlapAuto` ramps
in from velocity 0.85, a hard-played note does too. A 0 → 100 sweep at velocity
127 changes the output by nothing at all (rms 0.08413 either way); at velocity
100 it moves peak 0.41 → 0.34.

**Thump is disabled by Mute and by Legato.** `extraThump` is pinned to 1.5 when
Mute is on, and the whole term is multiplied by `(1 - articulationLegato)`.
With neither engaged the knob is strong — attack rms 0.048 → 0.205 across its
range.

`./scripts/render.sh` renders both as A/B pairs, alongside the styles, Ring and
Retrigger.

---

## How it was ported

The synth **is** Punk Labs' Faust code. `src/dsp/faust/bass.dsp` (the string)
and `output.dsp` (EQ → saturation → panner) sit beside their `onetrick.lib` and
`params.lib`. `scripts/gen_dsp.sh` compiles them to C++ with upstream's own
build line, `-lang cpp` in place of `-lang rust`, into `src/dsp/generated/` —
machine output, never hand-edited.

What this repo replaces is upstream's nih-plug/egui host.
`src/dsp/chonk_plugin.cpp` is the `plugin_api_v2` wrapper: the voice logic
(transcribed from `OneTrickMonoVoice`, edge cases included), the MIDI surface,
and the parameter surface Schwung reads.

**Three changes were made to the DSP**, all marked `SCHWUNG PORT ADDITION`.

The first is the **Ring** release control. Upstream damps the released string
to a fixed `bounceEfficiencyMuted = 0.5` with no control over it; Ring
interpolates the release efficiency between that and the open value, and does
it **geometrically in the decay rate** — which is linear in the log of the ring
time, so every step of the knob multiplies the ring by the same ratio.

That mapping took two goes, and the first one is worth recording. Interpolating
the log of the two *efficiencies* is linear in the **rate**, and ring time is
`1/rate`, so the time came out a hyperbola: 7× the minimum at 90% of the knob,
then a 47× jump in the last tenth. Both endpoints were exactly right either
way, so every test stayed green — it just played dead, which is the only way an
error in a *mapping* ever shows up. Measured, to -40 dB after release:

| ring | 0 | 10 | 25 | 50 | 75 | 90 | 100 |
|---|---|---|---|---|---|---|---|
| before | 0.12 s | 0.15 | 0.15 | 0.21 | 0.39 | 0.85 | **5.80** |
| after | 0.12 s | 0.18 | 0.33 | 1.04 | 3.36 | 5.80 | 5.80 |

At 0 it returns upstream's release to the sample; at 100 the release stops
damping entirely, so a released note decays exactly as a held one does. The top
of the knob flattens because the string's own Sustain is the ceiling — Ring
decides how much of it you keep, it cannot add more.

The second is **alternate picking**: a `Pick_Up` entry the wrapper writes one
stroke at a time, driving the excitation's sign, its energy and the transient
filter's corners. Deliberately unsmoothed — it has to be right at the trigger
edge, and a ramp between strokes would make every pluck half of each.

The third splits **Glide** and **Frets** out of Legato. Upstream welded three
behaviours together: `stringSlide` glided only under `articulationLegato`, and
`quantizeFrets` fretted only under Legato or an active slide pad. They are now
their own controls, with `glideTerm = max(legato, glide)`, `articulationFrets`
defaulting on, and the hard-coded 35 ms replaced by a `glideTime` slider that
defaults to 35 — so every combination that existed before behaves as it did.

**Retrigger** needed no DSP change but is not a flag either: Faust fires the
pluck on a *rising edge* of `Trigger`, which a block-rate host cannot
manufacture by writing the zone twice between blocks. The wrapper drops the
zone to 0 and lets `render_block` split one frame off the front and raise it
after. The pluck lands a sample late; no audio is lost, unlike upstream's
`burn_sample`, which discards the frame it advances.

The original source drop is kept under `original source/` for provenance.

## Building

```sh
./scripts/build.sh      # cross-compile for Move via Docker -> dist/chonk-module.tar.gz
./scripts/install.sh    # deploy to ableton@move.local (MOVE_HOST to override)
./scripts/test.sh       # host-side tests + help lint, no device needed
./scripts/render.sh     # audition/A-B WAV -> build/chonk_demo.wav
./scripts/gen_dsp.sh    # regenerate the C++ from the .dsp sources (brew install faust)
```

Faust is needed only to regenerate the DSP; the build container carries a cross
compiler and nothing else.

Two files are generated from the wrapper and are regenerated by `build.sh`:
`src/module.json`'s `ui_hierarchy`, and `src/dsp/factory_bank.h` — which is
indexed by `PARAMS[]` **position**, so adding a parameter without regenerating
loads every preset value past it one slot out, silently.

## Credits and license

**OneTrick CHONK** — a physically modeled electric bass monosynth.
Copyright © 2024 **Punk Labs LLC** · <https://punklabs.com/ot-chonk> ·
<contact@punklabs.com>

Move port by legsmechanical.

Released under the **GNU General Public License v3.0 or later**, inherited from
OneTrick CHONK. `LICENSE` is upstream's copy;
`original source/OneTrickCHONK-Source-v1.0.0/NOTICES.txt` carries the full
third-party acknowledgments — Faust's standard libraries (Julius O. Smith III's
filter and waveguide work among them), nih-plug, and the rest.

    This program is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the Free
    Software Foundation, either version 3 of the License, or (at your option)
    any later version.

    This program is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
    or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
    for more details.
