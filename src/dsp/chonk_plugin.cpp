/*
 * CHONK — Schwung plugin_api_v2 wrapper around OneTrick CHONK's Faust DSP.
 *
 * Upstream: OneTrick CHONK 1.0.0, Punk Labs LLC, GPL-3.0-or-later
 * (https://punklabs.com/ot-chonk). The synth IS the Faust code: bass.dsp is a
 * waveguide model of a single bass string (bridge/nut terminations, a pickup
 * tapped part-way down, a finger pluck and a noise transient), output.dsp is
 * the 5-band EQ + saturation + panner behind it. Both are compiled to C++ by
 * scripts/gen_dsp.sh and committed under generated/ — never hand-edited.
 *
 * What this file replaces is upstream's nih-plug/egui host: the voice logic,
 * the MIDI surface and the parameter surface Schwung reads.
 *
 * VOICE MODEL — mono, and that is not a simplification.
 * One waveguide, one string. Upstream's OneTrickMonoVoice keeps a stack of
 * held notes; a new note takes the string over, a release hands it back to the
 * note below (note_change, no re-pluck — that IS the legato/slide behaviour).
 * The stack semantics here are copied from that file, including the detail
 * that a release only re-pitches when the note released was the TOP one.
 *
 * TRIGGER is edge-detected inside Faust (triggerAndVelocity fires on a rising
 * edge only), so the zone must return to 0 before it can fire again. Upstream
 * does that in note_off (untrigger), and so do we: a repeated note while the
 * gate is still held deliberately does not re-pluck at the same velocity.
 */

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <new>

#include "faust_shim.h"
#include "generated/dsp_bass.hpp"
#include "generated/dsp_output.hpp"
#include "factory_bank.h"     /* CHONK_FACTORY[] — generated from Punk Labs' .preset files */

/* ---- host / plugin ABI (mirrors src/host/plugin_api_v1.h in the host) ---- */
#ifndef MOVE_SAMPLE_RATE
#define MOVE_SAMPLE_RATE 44100
#endif
#ifndef MOVE_FRAMES_PER_BLOCK
#define MOVE_FRAMES_PER_BLOCK 128
#endif

typedef struct host_api_v1 {
    uint32_t api_version;
    int sample_rate;
    int frames_per_block;
    uint8_t *mapped_memory;
    int audio_out_offset;
    int audio_in_offset;
    void (*log)(const char *msg);
    int (*midi_send_internal)(const uint8_t *msg, int len);
    int (*midi_send_external)(const uint8_t *msg, int len);
    int (*get_clock_status)(void);
    void *mod_emit_value;
    void *mod_clear_source;
    void *mod_host_ctx;
    float (*get_bpm)(void);
    int (*midi_inject_to_move)(const uint8_t *msg, int len);
    int (*slot_recv_channel)(void *instance);
    /* Appended by the host after slot_recv_channel; unused here, but the tail
     * is carried so this copy stays layout-identical to the real header. */
    double (*get_beat_position)(void);
    int (*midi_send_internal_slot)(int slot, const uint8_t *msg, int len);
    int (*clock_output_enabled)(void);
} host_api_v1_t;

typedef struct plugin_api_v2 {
    uint32_t api_version;
    void *(*create_instance)(const char *module_dir, const char *json_defaults);
    void (*destroy_instance)(void *instance);
    void (*on_midi)(void *instance, const uint8_t *msg, int len, int source);
    void (*set_param)(void *instance, const char *key, const char *val);
    int (*get_param)(void *instance, const char *key, char *buf, int buf_len);
    int (*get_error)(void *instance, char *buf, int buf_len);
    void (*render_block)(void *instance, int16_t *out_interleaved_lr, int frames);
} plugin_api_v2_t;

#define MOVE_PLUGIN_API_VERSION_2 2

static const host_api_v1_t *g_host = NULL;

/* ======================================================================== *
 *  Note layout
 *
 *  bass.dsp clamps the string to keys 24..67 of its own scale, which is
 *  MIDI 48..91 (C3..G6) — below that the waveguide runs out of delay line,
 *  above it the model goes unstable. Upstream enforces the same window in its
 *  plugin and puts the seven articulation keyswitches in the octave under it.
 *  Those notes are consumed, never played.
 * ======================================================================== */
#define CHONK_MIN_NOTE 48   /* C3  */
#define CHONK_MAX_NOTE 91   /* G6  */

#define KS_MUTE       36    /* C2  */
#define KS_SLIDE_DOWN 37    /* C#2 */
#define KS_FINGER     38    /* D2  */
#define KS_SLIDE_UP   39    /* D#2 */
#define KS_PICK       40    /* E2  */
#define KS_SLAP       41    /* F2  */
#define KS_LEGATO     42    /* F#2 */
#define KS_RING       43    /* G2  — this port's Ring/Gate, next pad up */
#define KS_ALTPICK    44    /* G#2 — this port's Alternate Picking */

#define CHONK_MAX_HELD 32

/* ======================================================================== *
 *  Parameter surface
 *
 *  Values are stored and exchanged in DISPLAY units — the same numbers the
 *  .preset files carry and the same ones the Shadow UI shows — and written
 *  straight into the Faust zone, because params.lib already declares its
 *  sliders in those units (percent, dB, cents, semitones). There is no
 *  normalized layer to get wrong.
 * ======================================================================== */
enum ParamKind {
    K_NUM,      /* numeric, display == zone value                       */
    K_TOGGLE,   /* latched articulation: Off/On enum -> 0.0/1.0 button  */
    K_STYLE,    /* the one-of-four playing style (see kStyleOpts)       */
    K_HOST      /* wrapper-side only: no Faust zone (velocity sens)     */
};

/* Finger / Pick / Slap are ONE CHOICE, not three switches. params.lib folds
 * them with max():
 *     articulationStyleAmount   = max(finger, pick, slap)
 *     articulationStyleHardness = max(pick * 1/2, slap)
 * so holding two just means the harder one wins and the other is inaudible.
 * Declaring them as three independent toggles offered a state the DSP cannot
 * represent; one enum is what the model actually has. */
static const char *kStyleOpts[] = {"Off", "Finger", "Pick", "Slap"};
enum { STYLE_OFF, STYLE_FINGER, STYLE_PICK, STYLE_SLAP, STYLE_COUNT };

typedef struct {
    const char *key;      /* Schwung param key                   */
    const char *name;     /* label shown on the device           */
    const char *zone;     /* Faust label, or NULL for K_HOST     */
    ParamKind   kind;
    float       min, max, def;
    const char *unit;     /* "%", "dB", "st", "cents", or NULL   */
    /* Switches a PRESET should put back to their default. Upstream's own
     * articulation (style, mute, legato) is left alone — that is how you are
     * playing, not what the patch is — but this port's switches are part of
     * the sound, so a preset owns them. Appended, because
     * tools/gen_factory_bank.mjs parses this table up to `def`. */
    int         reset_on_preset;
} param_def_t;

/* Order is the order the state blob is written in; it is not the UI order
 * (that is kUiHierarchy's job). */
static const param_def_t PARAMS[] = {
    /* --- Bass (bass.dsp) --- */
    {"pickup",   "Pickup Pos",  "Pickup_Position", K_NUM,    0,   100,  50, "%"},
    {"sustain",  "Sustain",     "Sustain",         K_NUM,    0,   100, 100, "%"},
    {"bright",   "Brightness",  "Brightness",      K_NUM,    0,   100,  50, "%"},
    {"strike",   "Strike Hard", "StrikeHardness",  K_NUM,    0,   100,   0, "%"},
    {"thump",    "Thump",       "Bass_Thump",      K_NUM,    0,   100,  50, "%"},
    {"tone",     "Tone",        "Tone_Knob",       K_NUM,    0,   100,  50, "%"},
    /* Ring is this port's addition to bass.dsp — how far the release is
     * allowed to let the string keep sounding. 0 is upstream's release. */
    {"ring",     "Ring",        "Ring",            K_NUM,    0,   100,   0, "%"},

    /* --- Mix (output.dsp) --- */
    {"gain",     "Volume",      "Mix_Gain",        K_NUM,  -60,     6,   0, "dB"},
    {"pan",      "Pan",         "Mix_Pan",         K_NUM, -100,   100,   0, "%"},
    {"sat",      "Saturation",  "Mix_Saturation",  K_NUM,    0,   100,   0, "%"},

    /* --- EQ (output.dsp): 100 Hz shelf, 250/500/1500 peaks, 3 kHz shelf --- */
    {"eq1",      "EQ 100Hz",    "EQ_Band_1",       K_NUM,  -12,    12,   0, "dB"},
    {"eq2",      "EQ 250Hz",    "EQ_Band_2",       K_NUM,  -12,    12,   0, "dB"},
    {"eq3",      "EQ 500Hz",    "EQ_Band_3",       K_NUM,  -12,    12,   0, "dB"},
    {"eq4",      "EQ 1.5kHz",   "EQ_Band_4",       K_NUM,  -12,    12,   0, "dB"},
    {"eq5",      "EQ 3kHz",     "EQ_Band_5",       K_NUM,  -12,    12,   0, "dB"},

    /* --- MIDI response --- */
    {"fine",     "Fine Tune",   "Fine_Tune",       K_NUM, -100,   100,   0, "cents"},
    {"at_range", "AT Range",    "Midi_Afterouch_range", K_NUM, -2, 2,    1, "st"},
    {"pw_range", "Bend Range",  "Midi_PitchWheel_range", K_NUM, 0, 12,   2, "st"},
    {"sens",     "Vel Sens",    NULL,              K_HOST,   0,   100, 100, "%"},

    /* --- Articulation, latched.
     * The same Faust buttons the keyswitches drive; the zone gets whichever of
     * the two is on, so a latched Mute and a held C2 do not fight (see
     * apply_artic). Slide Up/Down are deliberately NOT here: they are a
     * velocity-scaled ramp that runs for as long as the key is held, so a
     * latched one would climb 48 semitones and stay there. --- */
    /* Numeric literals, not the STYLE_* names: tools/gen_factory_bank.mjs
     * parses this table and indexes the bank by row, so a row it cannot read
     * silently shortens every preset. 0..3 == Off/Finger/Pick/Slap. */
    {"style",    "Style",       NULL,                  K_STYLE,  0,     3,   0, NULL},
    {"mute",     "Mute",        "Articulation_Mute",   K_TOGGLE, 0, 1, 0, NULL},
    {"legato",   "Legato",      "Articulation_Legato", K_TOGGLE, 0, 1, 0, NULL},
    /* Glide and Frets are the two halves of Legato that upstream welded to it.
     * Legato still implies glide (params.lib: glideTerm = max of the two), so
     * Legato alone sounds as it always has; Glide adds the slide while KEEPING
     * the click. Frets defaults ON, which is upstream's behaviour for every
     * case that existed before the switch. */
    {"glide",    "Glide",       "Articulation_Glide",  K_TOGGLE, 0, 1, 0, NULL, 1},
    /* Upstream's glide was a hard-coded 35 ms T60; that is the default here,
     * so an untouched patch glides as it always did. Higher = slower. The DSP
     * multiplies this by glideTerm, so with Glide and Legato both off the
     * pitch jumps whatever this says. */
    {"glide_ms", "Glide Time",  "Glide_Time",          K_NUM,    0,   500,  35, "ms"},
    {"frets",    "Frets",       "Articulation_Frets",  K_TOGGLE, 0, 1, 1, NULL, 1},
    /* Wrapper-side: there is no zone to hold it. What reaches the DSP is
     * Pick_Up, one stroke at a time, from voice_note_on. */
    {"altpick",  "Alt Pick",    NULL,                  K_TOGGLE, 0, 1, 0, NULL, 1},
    /* How far apart the strokes are: 100 is the voicing tuned by ear, 0 leaves
     * only the direction flip, 200 doubles the separation. Scales the three
     * magnitudes, never pickSign — see params.lib. */
    {"depth",    "Pick Depth",  "Pick_Depth",          K_NUM,    0,   200, 100, "%"},
    /* Wrapper-side: nothing in the DSP to write, it changes how Trigger is
     * driven. See voice_note_on and v2_render_block. */
    {"retrig",   "Retrigger",   NULL,                  K_TOGGLE, 0, 1, 0, NULL, 1},
};
#define CHONK_PARAM_COUNT ((int)(sizeof(PARAMS) / sizeof(PARAMS[0])))

static const param_def_t *find_param(const char *key) {
    for (int i = 0; i < CHONK_PARAM_COUNT; i++)
        if (strcmp(PARAMS[i].key, key) == 0) return &PARAMS[i];
    return NULL;
}

/* The latched on/off articulations. Style is not among them: it is one enum,
 * and its keyswitches write that enum instead (see style_from_key). */
enum { A_MUTE, A_LEGATO, A_ALTPICK, A_COUNT };
static const char *kArticKey[A_COUNT] = {"mute", "legato", "altpick"};

/* ======================================================================== *
 *  Instance
 * ======================================================================== */
typedef struct {
    DSP_Bass   bass;
    DSP_Output out;
    ZoneMap    bass_zones;
    ZoneMap    out_zones;

    /* Resolved once at create: two pointers per param, because Mix/EQ live in
     * output.dsp and the rest in bass.dsp, and params.lib is imported by both
     * (Sustain, for instance, only materializes in the one that reads it). */
    FAUSTFLOAT *zone_bass[CHONK_PARAM_COUNT];
    FAUSTFLOAT *zone_out[CHONK_PARAM_COUNT];
    float       value[CHONK_PARAM_COUNT];      /* display units */

    /* voice inputs (bass.dsp) */
    FAUSTFLOAT *z_gate, *z_gain, *z_key, *z_freq, *z_trigger, *z_wake;
    FAUSTFLOAT *z_pitchwheel, *z_modwheel, *z_aftertouch, *z_transpose;
    FAUSTFLOAT *z_slide_up, *z_slide_down, *z_let_ring;
    FAUSTFLOAT *z_out_wake, *z_out_modwheel;

    uint8_t held[CHONK_MAX_HELD];   /* note stack, oldest first */
    int     held_count;
    int     gate_count;             /* upstream's param_gate_counter */
    uint8_t sustained[CHONK_MAX_HELD];
    int     sustained_count;
    int     sustain_pedal;

    int     artic_key[A_COUNT];     /* held keyswitch */
    int     style_key;              /* style keyswitch held, or -1 */
    float   pending_trigger;        /* velocity waiting for a forced edge, or -1 */
    int     next_up;                /* 1 if the NEXT stroke is an upstroke */
    int     idle_frames;            /* rendered frames since the last note-on */
    FAUSTFLOAT *z_pick_up;
    FAUSTFLOAT *z_style[STYLE_COUNT];  /* NULL, finger, pick, slap */
    int     octave_transpose;
    int     cur_preset;             /* index into CHONK_FACTORY, -1 = Init */
    char    module_dir[512];

    float   mono[MOVE_FRAMES_PER_BLOCK * 4];
    float   outL[MOVE_FRAMES_PER_BLOCK * 4];
    float   outR[MOVE_FRAMES_PER_BLOCK * 4];
} chonk_t;

static float clampf(float v, float lo, float hi) { return v < lo ? lo : (v > hi ? hi : v); }

static void write_zone(chonk_t *inst, int i, float display) {
    if (inst->zone_bass[i]) *inst->zone_bass[i] = (FAUSTFLOAT)display;
    if (inst->zone_out[i])  *inst->zone_out[i]  = (FAUSTFLOAT)display;
}

/* A held keyswitch OVERRIDES its latched parameter — it does not merely add to
 * it. Upstream only had the keyswitch; the latch is this port's addition, for a
 * device whose pads are also its keyboard, and OR-ing the two made the pad a
 * no-op whenever the latch was on: with Mute latched there was no way to play
 * one open note. Inverting instead makes the pad meaningful in both directions
 * and keeps the old behaviour intact wherever the latch is off:
 *
 *     latch off + pad held -> on   (unchanged)
 *     latch on  + pad held -> off  (the pad lifts it for as long as you hold)
 *
 * This matches what the Style keyswitches already do to the Style enum, so
 * every articulation pad now means the same thing: while I am held, I am the
 * one deciding. */
static void apply_artic(chonk_t *inst, int a) {
    const param_def_t *p = find_param(kArticKey[a]);
    if (!p) return;
    int idx = (int)(p - PARAMS);
    int latched = (inst->value[idx] >= 0.5f);
    int on = inst->artic_key[a] ? !latched : latched;
    write_zone(inst, idx, on ? 1.0f : 0.0f);
}

/* One of the three style buttons is up, the rest are down. A held keyswitch
 * wins over the latched enum for as long as it is held — the pad is the
 * gesture, the enum is the setting. */
static void apply_style(chonk_t *inst) {
    const param_def_t *p = find_param("style");
    if (!p) return;
    int latched = (int)inst->value[(int)(p - PARAMS)];
    int eff = (inst->style_key >= 0) ? inst->style_key : latched;
    for (int st = STYLE_FINGER; st <= STYLE_SLAP; st++)
        if (inst->z_style[st]) *inst->z_style[st] = (st == eff) ? 1.0f : 0.0f;
}

static void set_value(chonk_t *inst, int i, float display) {
    const param_def_t *p = &PARAMS[i];
    float v = clampf(display, p->min, p->max);
    inst->value[i] = v;
    if (p->kind == K_HOST) return;
    if (p->kind == K_STYLE) { apply_style(inst); return; }
    if (p->kind == K_TOGGLE) {
        /* The ones with a pad go through apply_artic, which folds the pad in. */
        for (int a = 0; a < A_COUNT; a++)
            if (strcmp(p->key, kArticKey[a]) == 0) { apply_artic(inst, a); return; }
        /* The ones without a pad are written straight. This fallthrough is
         * load-bearing: before Glide and Frets existed every toggle had a pad,
         * and a `return` here would have left their zones at 0 forever with
         * nothing reporting a problem. */
        write_zone(inst, i, v >= 0.5f ? 1.0f : 0.0f);
        return;
    }
    write_zone(inst, i, v);
}

/* ======================================================================== *
 *  Voice — transcribed from onetrick/src/faust/wrapper.rs (OneTrickDSP) and
 *  OneTrickMonoVoice. Keep the shapes; the edge cases are load-bearing.
 * ======================================================================== */
static float note_to_freq(float note) { return 440.0f * powf(2.0f, (note - 69.0f) / 12.0f); }

static void voice_wake(chonk_t *inst) {
    /* Upstream burns one sample with the gate down and WakeUp toggled so the
     * smoothed parameters restart from their real values instead of ramping up
     * from whatever they decayed to while idle. We keep WakeUp pinned on: the
     * Schwung host renders this module continuously, so there is no idle state
     * to come back from, and the burn would cost a sample of audio per note. */
    if (inst->z_wake) *inst->z_wake = 1.0f;
    if (inst->z_out_wake) *inst->z_out_wake = 1.0f;
}

static void voice_note_change(chonk_t *inst, float note) {
    if (inst->z_key) *inst->z_key = (FAUSTFLOAT)note;
    if (inst->z_freq) *inst->z_freq = (FAUSTFLOAT)note_to_freq(note);
}

/* Alternate picking is on if the latch says so, or the pad is held and it does
 * not — the same invert every articulation pad does. */
static int altpick_on(const chonk_t *inst) {
    const param_def_t *p = find_param("altpick");
    if (!p) return 0;
    int latched = (inst->value[(int)(p - PARAMS)] >= 0.5f);
    return inst->artic_key[A_ALTPICK] ? !latched : latched;
}

/* Which way the pick is travelling for THIS note. A phrase starts on a
 * downstroke, and the count restarts after a PAUSE (kAltPickResetMs of no new
 * note) rather than whenever the string falls silent.
 *
 * ⚠ That distinction is the whole feature. Picking is normally separate notes
 * — press, release, press — so resetting on release, which is what this did
 * first, made every note a downstroke and alternation did nothing at all
 * except under a held legato line. The demo render is what caught it. */
/* How much SILENCE reads as a new phrase. A second is long enough that no
 * groove crosses it and short enough that a deliberate stop does. */
#define kAltPickResetMs 1000

static void advance_pick(chonk_t *inst) {
    inst->idle_frames = 0;
    int up = altpick_on(inst) ? inst->next_up : 0;
    if (inst->z_pick_up) *inst->z_pick_up = (FAUSTFLOAT)up;
    inst->next_up = altpick_on(inst) ? !up : 0;
}

static int retrig_on(const chonk_t *inst) {
    const param_def_t *p = find_param("retrig");
    return p && inst->value[(int)(p - PARAMS)] >= 0.5f;
}

static void voice_note_on(chonk_t *inst, float note, float velocity) {
    inst->gate_count++;
    voice_wake(inst);
    /* Before the trigger: Pick_Up is not smoothed, so it has to be right at
     * the edge rather than ramping into it. One stroke per note-on, whether or
     * not Faust's edge detector ends up plucking — the pick moved either way. */
    advance_pick(inst);
    /*
     * Faust edge-detects this: triggerAndVelocity fires only while
     * Trigger > Trigger'. Writing the same velocity twice between blocks is
     * therefore ONE value as far as the DSP is concerned, and the second note
     * does not pluck — which is upstream's behaviour and is usually what you
     * want under the fingers.
     *
     * Retrigger forces the edge the only way a block-rate host can: drop the
     * zone to 0 now, and let render_block raise it after ONE frame. That costs
     * a sample of delay on the pluck, not a sample of audio — the frame is
     * rendered into the output like any other, unlike upstream's burn_sample
     * hack, which discards one.
     */
    if (retrig_on(inst)) {
        if (inst->z_trigger) *inst->z_trigger = 0.0f;
        inst->pending_trigger = (float)velocity;
    } else {
        if (inst->z_trigger) *inst->z_trigger = (FAUSTFLOAT)velocity;
        inst->pending_trigger = -1.0f;
    }
    if (inst->z_gate) *inst->z_gate = 1.0f;
    if (inst->z_gain) *inst->z_gain = (FAUSTFLOAT)velocity;
    voice_note_change(inst, note);
}

static void voice_gate_off(chonk_t *inst) {
    /* note_off_internal: only when nothing is left holding the gate. Clearing
     * Trigger here is what re-arms Faust's rising edge for the next pluck. */
    if (inst->gate_count == 0) {
        if (inst->z_trigger) *inst->z_trigger = 0.0f;
        if (inst->z_gate) *inst->z_gate = 0.0f;
    }
}

static int held_index(const chonk_t *inst, uint8_t note) {
    for (int i = 0; i < inst->held_count; i++)
        if (inst->held[i] == note) return i;
    return -1;
}

static void held_remove_at(chonk_t *inst, int idx) {
    for (int i = idx; i < inst->held_count - 1; i++) inst->held[i] = inst->held[i + 1];
    inst->held_count--;
}

static void mono_note_on(chonk_t *inst, uint8_t note, float velocity) {
    int idx = held_index(inst, note);
    if (idx >= 0) held_remove_at(inst, idx);
    if (inst->held_count < CHONK_MAX_HELD) inst->held[inst->held_count++] = note;
    voice_note_on(inst, (float)note, velocity);
}

static void mono_note_off(chonk_t *inst, uint8_t note) {
    int idx = held_index(inst, note);
    if (idx < 0) return;
    int was_top = (idx == inst->held_count - 1);
    /* clear_gate_counter: a release drops the gate for the whole stack, not
     * one increment of it — that is upstream's behaviour and it is why a fast
     * roll of notes does not leave the string ringing. */
    inst->gate_count = 0;
    if (inst->held_count == 1) {
        inst->held_count = 0;
        voice_gate_off(inst);
        return;
    }
    held_remove_at(inst, idx);
    if (was_top && inst->held_count > 0)
        voice_note_change(inst, (float)inst->held[inst->held_count - 1]);
}

static void all_notes_off(chonk_t *inst) {
    inst->pending_trigger = -1.0f;
    inst->next_up = 0;
    inst->idle_frames = 0;
    if (inst->z_pick_up) *inst->z_pick_up = 0.0f;
    inst->held_count = 0;
    inst->sustained_count = 0;
    inst->gate_count = 0;
    voice_gate_off(inst);
    if (inst->z_slide_up) *inst->z_slide_up = 0.0f;
    if (inst->z_slide_down) *inst->z_slide_down = 0.0f;
    if (inst->z_let_ring) *inst->z_let_ring = 0.0f;
    for (int a = 0; a < A_COUNT; a++) { inst->artic_key[a] = 0; apply_artic(inst, a); }
    inst->style_key = -1;
    apply_style(inst);
}

/* ======================================================================== *
 *  Presets
 * ======================================================================== */
static int preset_count(void) { return CHONK_FACTORY_COUNT; }

static void load_preset(chonk_t *inst, int idx) {
    if (idx < 0 || idx >= CHONK_FACTORY_COUNT) return;
    const chonk_preset_t *pr = &CHONK_FACTORY[idx];
    /* A preset carries the Bass/Mix/EQ/MIDI surface. Upstream's articulation
     * (style, mute, legato) is performance state and is left where the player
     * put it; the switches this port added are part of the patch, so a preset
     * puts them back to their defaults — see reset_on_preset. */
    for (int i = 0; i < CHONK_PARAM_COUNT; i++) {
        if (PARAMS[i].kind == K_TOGGLE || PARAMS[i].kind == K_STYLE) {
            if (PARAMS[i].reset_on_preset) set_value(inst, i, PARAMS[i].def);
            continue;
        }
        set_value(inst, i, pr->value[i]);
    }
    inst->cur_preset = idx;
}

static void load_defaults(chonk_t *inst) {
    for (int i = 0; i < CHONK_PARAM_COUNT; i++) set_value(inst, i, PARAMS[i].def);
    inst->cur_preset = -1;
}

/* ======================================================================== *
 *  UI surface
 * ======================================================================== */
static const char *kUiHierarchy =
"{\"modes\":null,\"levels\":{"
 /*
  * ROOT IS THE STRING PAGE, and the children split BY HAND rather than by
  * kind. Style, Strike, Mute, Alt Pick and Pick Depth are all the striking
  * hand; Legato, Glide, Glide Time, Frets and Retrigger are all what happens
  * between notes. That is how a bass player divides them, and it falls out
  * 5 and 5.
  *
  * Two things this layout fixes:
  *
  *  - A separate String level used to hold root's own knob row minus Volume.
  *    It earned nothing, and one edit from being identical it would have
  *    rendered EMPTY -- the planner dedupes on a level's whole authored knob
  *    signature and visits root first.
  *  - Style and Strike Hardness were on different pages while being the SAME
  *    control: a style does nothing but replace Strike with a fixed value
  *    (Finger 0, Pick 50, Slap 100). Split across pages, the knob reads as
  *    broken. They are adjacent now.
  *
  * Saturation and Volume are on root's knob row but live on Output. A level's
  * knobs need not appear in its own params, so the row can be what you play
  * while the menu stays the string and four doors.
  */
 "\"root\":{\"label\":\"CHONK\","
   "\"list_param\":\"preset\",\"count_param\":\"preset_count\",\"name_param\":\"preset_name\","
   "\"knobs\":[\"pickup\",\"bright\",\"tone\",\"thump\",\"sustain\",\"ring\",\"sat\",\"gain\"],"
   "\"params\":["
     "\"pickup\",\"bright\",\"tone\",\"thump\",\"sustain\",\"ring\","
     "{\"level\":\"attack\",\"label\":\"Attack\"},"
     "{\"level\":\"fretting\",\"label\":\"Fretting\"},"
     "{\"level\":\"output\",\"label\":\"Output\"},"
     "{\"level\":\"midi\",\"label\":\"MIDI\"}"
   "]},"
 "\"attack\":{\"name\":\"Attack\",\"knobs\":[\"style\",\"strike\",\"mute\",\"altpick\",\"depth\"],"
   "\"params\":[\"style\",\"strike\",\"mute\",\"altpick\",\"depth\"]},"
 "\"fretting\":{\"name\":\"Fretting\",\"knobs\":[\"legato\",\"glide\",\"glide_ms\",\"frets\",\"retrig\"],"
   "\"params\":[\"legato\",\"glide\",\"glide_ms\",\"frets\",\"retrig\"]},"
 "\"output\":{\"name\":\"Output\",\"knobs\":[\"gain\",\"pan\",\"sat\",\"eq1\",\"eq2\",\"eq3\",\"eq4\",\"eq5\"],"
   "\"params\":[\"gain\",\"pan\",\"sat\",\"eq1\",\"eq2\",\"eq3\",\"eq4\",\"eq5\"]},"
 "\"midi\":{\"name\":\"MIDI\",\"knobs\":[\"fine\",\"pw_range\",\"at_range\",\"sens\"],"
   "\"params\":[\"fine\",\"pw_range\",\"at_range\",\"sens\"]}"
"}}";

static int build_chain_params(char *buf, int len) {
    int n = 0;
    n += snprintf(buf + n, len - n, "[");
    for (int i = 0; i < CHONK_PARAM_COUNT && n < len - 256; i++) {
        const param_def_t *p = &PARAMS[i];
        if (i) n += snprintf(buf + n, len - n, ",");
        if (p->kind == K_STYLE) {
            n += snprintf(buf + n, len - n,
                "{\"key\":\"%s\",\"name\":\"%s\",\"type\":\"enum\",\"options\":[", p->key, p->name);
            for (int o = 0; o < STYLE_COUNT; o++)
                n += snprintf(buf + n, len - n, "%s\"%s\"", o ? "," : "", kStyleOpts[o]);
            n += snprintf(buf + n, len - n, "],\"default\":\"Off\"}");
            continue;
        }
        if (p->kind == K_TOGGLE) {
            n += snprintf(buf + n, len - n,
                "{\"key\":\"%s\",\"name\":\"%s\",\"type\":\"enum\",\"options\":[\"Off\",\"On\"],\"default\":\"Off\"}",
                p->key, p->name);
            continue;
        }
        /* Integer steps everywhere: every one of these ranges is already
         * coarse enough to sweep with a knob, and params.lib's 0.01 steps
         * exist for a mouse, not for Move's encoders. `max` is declared on
         * percent params deliberately — without it the shared formatter
         * assumes 0..1 and renders 50 as 5000%. */
        n += snprintf(buf + n, len - n,
            "{\"key\":\"%s\",\"name\":\"%s\",\"type\":\"int\",\"min\":%d,\"max\":%d,\"default\":%d",
            p->key, p->name, (int)p->min, (int)p->max, (int)p->def);
        if (p->unit) n += snprintf(buf + n, len - n, ",\"unit\":\"%s\"", p->unit);
        n += snprintf(buf + n, len - n, "}");
    }
    n += snprintf(buf + n, len - n, "]");
    return n;
}

/* ---- slot state ---- */
static int json_get_number(const char *json, const char *key, float *out) {
    char search[64];
    snprintf(search, sizeof(search), "\"%s\":", key);
    const char *pos = strstr(json, search);
    if (!pos) return -1;
    pos += strlen(search);
    while (*pos == ' ') pos++;
    *out = (float)atof(pos);
    return 0;
}

static int build_state(chonk_t *inst, char *buf, int buf_len) {
    int n = snprintf(buf, buf_len, "{\"preset\":%d,\"octave_transpose\":%d",
                     inst->cur_preset, inst->octave_transpose);
    for (int i = 0; i < CHONK_PARAM_COUNT && n < buf_len - 64; i++)
        n += snprintf(buf + n, buf_len - n, ",\"%s\":%.4f", PARAMS[i].key, inst->value[i]);
    n += snprintf(buf + n, buf_len - n, "}");
    return n;
}

static void restore_state(chonk_t *inst, const char *json) {
    float f;
    if (json_get_number(json, "preset", &f) == 0) {
        int idx = (int)f;
        if (idx >= 0 && idx < preset_count()) load_preset(inst, idx);
        else inst->cur_preset = -1;
    }
    if (json_get_number(json, "octave_transpose", &f) == 0) inst->octave_transpose = (int)f;
    /* After the preset, so a value edited since the preset was picked wins —
     * the slot keeps the sound it was saved with. */
    for (int i = 0; i < CHONK_PARAM_COUNT; i++)
        if (json_get_number(json, PARAMS[i].key, &f) == 0) set_value(inst, i, f);
}

/* ======================================================================== *
 *  v2 entry points
 * ======================================================================== */
static void resolve_zones(chonk_t *inst) {
    for (int i = 0; i < CHONK_PARAM_COUNT; i++) {
        inst->zone_bass[i] = PARAMS[i].zone ? inst->bass_zones.find(PARAMS[i].zone) : nullptr;
        inst->zone_out[i]  = PARAMS[i].zone ? inst->out_zones.find(PARAMS[i].zone) : nullptr;
    }
    inst->z_gate       = inst->bass_zones.find("gate");
    inst->z_gain       = inst->bass_zones.find("gain");
    inst->z_key        = inst->bass_zones.find("key");
    inst->z_freq       = inst->bass_zones.find("freq");
    inst->z_trigger    = inst->bass_zones.find("Trigger");
    inst->z_wake       = inst->bass_zones.find("WakeUp");
    inst->z_pitchwheel = inst->bass_zones.find("PitchWheel");
    inst->z_modwheel   = inst->bass_zones.find("ModWheel");
    inst->z_aftertouch = inst->bass_zones.find("Aftertouch");
    inst->z_transpose  = inst->bass_zones.find("Transpose");
    inst->z_style[STYLE_OFF]    = nullptr;
    inst->z_style[STYLE_FINGER] = inst->bass_zones.find("Articulation_Finger");
    inst->z_style[STYLE_PICK]   = inst->bass_zones.find("Articulation_Pick");
    inst->z_style[STYLE_SLAP]   = inst->bass_zones.find("Articulation_Slap");
    inst->z_let_ring   = inst->bass_zones.find("Articulation_Ring");
    inst->z_pick_up    = inst->bass_zones.find("Pick_Up");
    inst->z_slide_up   = inst->bass_zones.find("Articulation_SlideUp");
    inst->z_slide_down = inst->bass_zones.find("Articulation_SlideDown");
    inst->z_out_wake     = inst->out_zones.find("WakeUp");
    inst->z_out_modwheel = inst->out_zones.find("ModWheel");
}

static void *v2_create_instance(const char *module_dir, const char *json_defaults) {
    chonk_t *inst = new (std::nothrow) chonk_t();
    if (!inst) return NULL;
    memset(inst->held, 0, sizeof(inst->held));
    inst->held_count = 0;
    inst->gate_count = 0;
    inst->sustained_count = 0;
    inst->sustain_pedal = 0;
    inst->octave_transpose = 0;
    inst->cur_preset = -1;
    for (int a = 0; a < A_COUNT; a++) inst->artic_key[a] = 0;
    inst->style_key = -1;
    inst->pending_trigger = -1.0f;
    inst->next_up = 0;
    inst->idle_frames = 0;
    if (module_dir) strncpy(inst->module_dir, module_dir, sizeof(inst->module_dir) - 1);

    int sr = (g_host && g_host->sample_rate > 0) ? g_host->sample_rate : MOVE_SAMPLE_RATE;
    DSP_Bass::classInit(sr);
    DSP_Output::classInit(sr);
    inst->bass.init(sr);
    inst->out.init(sr);
    inst->bass.buildUserInterface(&inst->bass_zones);
    inst->out.buildUserInterface(&inst->out_zones);
    resolve_zones(inst);

    load_defaults(inst);
    if (preset_count() > 0) load_preset(inst, 0);
    if (json_defaults) restore_state(inst, json_defaults);
    voice_wake(inst);
    return inst;
}

static void v2_destroy_instance(void *instance) {
    chonk_t *inst = (chonk_t *)instance;
    if (inst) delete inst;
}

static void v2_on_midi(void *instance, const uint8_t *msg, int len, int source) {
    chonk_t *inst = (chonk_t *)instance;
    if (!inst || len < 2) return;
    (void)source;
    uint8_t status = msg[0] & 0xF0;
    uint8_t d1 = msg[1];
    uint8_t d2 = (len > 2) ? msg[2] : 0;

    switch (status) {
        case 0x90:
        case 0x80: {
            int on = (status == 0x90 && d2 > 0);
            /* The octave shift moves the keyswitches with the notes: on Move
             * they are pads on the same grid, and a fixed C2 row would walk
             * off the bottom of it. */
            int note = (int)d1 + inst->octave_transpose * 12;
            if (note < 0 || note > 127) return;
            float vel = d2 / 127.0f;

            if (note >= CHONK_MIN_NOTE && note <= CHONK_MAX_NOTE) {
                if (on) {
                    /* A new note ends a slide in progress — upstream does this
                     * only when something is already held, and so do we. */
                    if (inst->held_count > 0) {
                        if (inst->z_slide_up) *inst->z_slide_up = 0.0f;
                        if (inst->z_slide_down) *inst->z_slide_down = 0.0f;
                    }
                    const param_def_t *sp = find_param("sens");
                    float sens = sp ? inst->value[(int)(sp - PARAMS)] * 0.01f : 1.0f;
                    vel = vel * sens + (1.0f - sens) * (80.0f / 127.0f);
                    mono_note_on(inst, (uint8_t)note, vel);
                } else {
                    if (inst->sustain_pedal) {
                        if (inst->sustained_count < CHONK_MAX_HELD)
                            inst->sustained[inst->sustained_count++] = (uint8_t)note;
                        break;
                    }
                    mono_note_off(inst, (uint8_t)note);
                    if (inst->z_slide_up) *inst->z_slide_up = 0.0f;
                    if (inst->z_slide_down) *inst->z_slide_down = 0.0f;
                }
                break;
            }

            /* Keyswitches. Slide is velocity-scaled while held; the rest are
             * momentary gates ORed with their latched parameter. */
            switch (note) {
                case KS_SLIDE_UP:
                    if (inst->z_slide_up) *inst->z_slide_up = on ? (FAUSTFLOAT)vel : 0.0f;
                    break;
                case KS_SLIDE_DOWN:
                    if (inst->z_slide_down) *inst->z_slide_down = on ? (FAUSTFLOAT)vel : 0.0f;
                    break;
                case KS_MUTE:   inst->artic_key[A_MUTE]     = on; apply_artic(inst, A_MUTE);     break;
                case KS_LEGATO: inst->artic_key[A_LEGATO]   = on; apply_artic(inst, A_LEGATO);   break;
                /* Momentary full ring. There is no latched counterpart: the
                 * Ring knob at 100 IS that latch (measured — with the knob up,
                 * a latched toggle changed the release tail by nothing), so a
                 * second control for the same value would only be a second
                 * place to look. */
                case KS_RING:   if (inst->z_let_ring) *inst->z_let_ring = on ? 1.0f : 0.0f; break;
                case KS_ALTPICK: inst->artic_key[A_ALTPICK] = on;
                                 apply_artic(inst, A_ALTPICK);
                                 /* Picking up or putting down the pad starts a
                                  * fresh alternation rather than continuing
                                  * whatever half-phrase was in progress. */
                                 inst->next_up = 0;
                                 break;
                /* A style keyswitch selects while held; releasing it hands the
                 * string back to whatever the Style enum says. Releasing a pad
                 * that is not the one currently held (a roll across three of
                 * them) must not clear the other's selection. */
                case KS_FINGER:
                case KS_PICK:
                case KS_SLAP: {
                    int st = (note == KS_FINGER) ? STYLE_FINGER
                           : (note == KS_PICK)   ? STYLE_PICK : STYLE_SLAP;
                    if (on) inst->style_key = st;
                    else if (inst->style_key == st) inst->style_key = -1;
                    apply_style(inst);
                    break;
                }
                default: break;
            }
            break;
        }
        case 0xD0:  /* channel pressure */
            if (inst->z_aftertouch) *inst->z_aftertouch = (FAUSTFLOAT)(d1 / 127.0f);
            break;
        case 0xA0:  /* poly pressure — one string, so it is channel pressure */
            if (inst->z_aftertouch) *inst->z_aftertouch = (FAUSTFLOAT)(d2 / 127.0f);
            break;
        case 0xE0: {
            if (len < 3) break;
            int bend = ((int)d2 << 7 | d1) - 8192;
            if (inst->z_pitchwheel) *inst->z_pitchwheel = (FAUSTFLOAT)(bend / 8192.0f);
            break;
        }
        case 0xB0: {
            switch (d1) {
                case 1:  /* mod wheel — opens the string's sustain right up */
                    if (inst->z_modwheel) *inst->z_modwheel = (FAUSTFLOAT)(d2 / 127.0f);
                    if (inst->z_out_modwheel) *inst->z_out_modwheel = (FAUSTFLOAT)(d2 / 127.0f);
                    break;
                case 64: {  /* sustain pedal. Upstream's DSP ignores it (its
                             * Sustain knob is a different thing); holding notes
                             * is the host convention, so the wrapper does it. */
                    int down = d2 >= 64;
                    inst->sustain_pedal = down;
                    if (!down) {
                        int n = inst->sustained_count;
                        inst->sustained_count = 0;
                        for (int i = 0; i < n; i++) mono_note_off(inst, inst->sustained[i]);
                    }
                    break;
                }
                case 68: {  /* legato pedal, as upstream */
                    inst->artic_key[A_LEGATO] = (d2 >= 64);
                    apply_artic(inst, A_LEGATO);
                    break;
                }
                case 120: case 123:
                    all_notes_off(inst);
                    break;
                default: break;
            }
            break;
        }
        default: break;
    }
}

static void v2_set_param(void *instance, const char *key, const char *val) {
    chonk_t *inst = (chonk_t *)instance;
    if (!inst || !key || !val) return;

    if (strcmp(key, "all_notes_off") == 0) { all_notes_off(inst); return; }
    if (strcmp(key, "octave_transpose") == 0) { inst->octave_transpose = atoi(val); return; }
    if (strcmp(key, "state") == 0) { restore_state(inst, val); return; }
    if (strcmp(key, "preset") == 0) {
        int idx = atoi(val);
        if (idx >= 0 && idx < preset_count() && idx != inst->cur_preset) load_preset(inst, idx);
        return;
    }

    const param_def_t *p = find_param(key);
    if (!p) return;
    int i = (int)(p - PARAMS);
    if (p->kind == K_STYLE) {
        for (int st = 0; st < STYLE_COUNT; st++)
            if (strcmp(val, kStyleOpts[st]) == 0) { set_value(inst, i, (float)st); return; }
        set_value(inst, i, (float)atoi(val));
        return;
    }
    if (p->kind == K_TOGGLE) {
        /* The shadow UI sends an enum as its index; accept "On"/"Off" too, so
         * a chain patch or a hand-written preset reads the way it looks. */
        float on = (strcmp(val, "On") == 0 || strcmp(val, "on") == 0 || atoi(val) != 0) ? 1.0f : 0.0f;
        set_value(inst, i, on);
        return;
    }
    set_value(inst, i, (float)atof(val));
}

static int v2_get_param(void *instance, const char *key, char *buf, int buf_len) {
    chonk_t *inst = (chonk_t *)instance;
    if (!inst || !key || !buf || buf_len <= 0) return -1;

    if (strcmp(key, "ui_hierarchy") == 0) return snprintf(buf, buf_len, "%s", kUiHierarchy);
    if (strcmp(key, "chain_params") == 0) return build_chain_params(buf, buf_len);
    if (strcmp(key, "name") == 0) return snprintf(buf, buf_len, "CHONK");
    if (strcmp(key, "state") == 0) return build_state(inst, buf, buf_len);
    if (strcmp(key, "preset_count") == 0) return snprintf(buf, buf_len, "%d", preset_count());
    if (strcmp(key, "preset") == 0)
        return snprintf(buf, buf_len, "%d", inst->cur_preset < 0 ? 0 : inst->cur_preset);
    if (strcmp(key, "preset_name") == 0) {
        if (inst->cur_preset < 0 || inst->cur_preset >= preset_count())
            return snprintf(buf, buf_len, "Init");
        return snprintf(buf, buf_len, "%s", CHONK_FACTORY[inst->cur_preset].name);
    }
    if (strcmp(key, "octave_transpose") == 0)
        return snprintf(buf, buf_len, "%d", inst->octave_transpose);
    /* The shared sound-generator UI puts this in the status line. One
     * waveguide, one string — see the voice-model note at the top. */
    if (strcmp(key, "polyphony") == 0) return snprintf(buf, buf_len, "1");

    const param_def_t *p = find_param(key);
    if (!p) return -1;   /* NEGATIVE for an unknown key: 0 would claim the value is "" */
    int i = (int)(p - PARAMS);
    if (p->kind == K_TOGGLE) return snprintf(buf, buf_len, "%d", inst->value[i] >= 0.5f ? 1 : 0);
    if (p->kind == K_STYLE)  return snprintf(buf, buf_len, "%d", (int)inst->value[i]);
    return snprintf(buf, buf_len, "%.3f", inst->value[i]);
}

static int v2_get_error(void *instance, char *buf, int buf_len) {
    (void)instance;
    if (buf && buf_len > 0) buf[0] = '\0';
    return 0;
}

static void v2_render_block(void *instance, int16_t *out_lr, int frames) {
    chonk_t *inst = (chonk_t *)instance;
    if (!inst || !out_lr || frames <= 0) return;

    const int kMax = (int)(sizeof(inst->mono) / sizeof(inst->mono[0]));
    int done = 0;

    /* A REST long enough to be a new phrase puts the pick back on a downstroke.
     *
     * "Rest" here means NOTHING IS HELD — hands off the pads — not silence.
     * The string can still be ringing for seconds with Ring up, and the clock
     * runs anyway, because that is a rest as a player means it. A held note is
     * never a rest at any length, and neither is a sustain pedal holding notes
     * (releases go to the sustained list without dropping held_count, so this
     * gets that for free).
     *
     * ⚠ It first counted from the last note-ON. A quarter note at 120 bpm is a
     * 500 ms gap, so every note reset and anything slower than eighths came out
     * all downstrokes; a note held past the threshold reset itself mid-ring. */
    int sr = (g_host && g_host->sample_rate > 0) ? g_host->sample_rate : MOVE_SAMPLE_RATE;
    if (inst->held_count > 0) {
        inst->idle_frames = 0;
    } else {
        if (inst->idle_frames < sr * 4) inst->idle_frames += frames;
        if (inst->idle_frames >= sr * kAltPickResetMs / 1000) inst->next_up = 0;
    }

    /* A forced retrigger: render exactly one frame with Trigger still at 0,
     * then raise it, so the next frame is a rising edge the DSP can see. */
    int split = (inst->pending_trigger >= 0.0f) ? 1 : 0;

    while (done < frames) {
        int n = frames - done;
        if (n > kMax) n = kMax;
        if (split) { n = 1; split = 0; }

        FAUSTFLOAT *bass_out[1] = {inst->mono};
        inst->bass.compute(n, nullptr, bass_out);

        FAUSTFLOAT *in[1] = {inst->mono};
        FAUSTFLOAT *outs[2] = {inst->outL, inst->outR};
        inst->out.compute(n, in, outs);

        for (int i = 0; i < n; i++) {
            float l = inst->outL[i], r = inst->outR[i];
            /* Hard clip at full scale: the waveguide can overshoot on a slap,
             * and wrapping an int16 turns that into a click. */
            l = clampf(l, -1.0f, 1.0f);
            r = clampf(r, -1.0f, 1.0f);
            out_lr[(done + i) * 2]     = (int16_t)lrintf(l * 32767.0f);
            out_lr[(done + i) * 2 + 1] = (int16_t)lrintf(r * 32767.0f);
        }
        done += n;

        if (inst->pending_trigger >= 0.0f) {
            if (inst->z_trigger) *inst->z_trigger = (FAUSTFLOAT)inst->pending_trigger;
            inst->pending_trigger = -1.0f;
        }
    }
}

static plugin_api_v2_t g_plugin_api_v2 = {
    MOVE_PLUGIN_API_VERSION_2,
    v2_create_instance,
    v2_destroy_instance,
    v2_on_midi,
    v2_set_param,
    v2_get_param,
    v2_get_error,
    v2_render_block,
};

extern "C" plugin_api_v2_t *move_plugin_init_v2(const host_api_v1_t *host) {
    g_host = host;
    return &g_plugin_api_v2;
}
