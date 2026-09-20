/*
 * Host-side tests for the CHONK wrapper: build native, run on the workstation.
 *
 *   ./scripts/test.sh
 *
 * The wrapper is #included rather than linked so the tests can reach the
 * static tables (PARAMS, CHONK_FACTORY) the module's behaviour is defined by.
 * Everything here is reachable without a device: the DSP is deterministic and
 * needs no audio hardware, so "does a note make a sound" is a real assertion
 * and not a stand-in for playing it.
 */
#include "../src/dsp/chonk_plugin.cpp"

#include <cstdio>
#include <cmath>
#include <string>
#include <vector>

static int g_fail = 0;
static int g_checks = 0;

static void ok(bool cond, const char *what) {
    g_checks++;
    if (!cond) { printf("  FAIL  %s\n", what); g_fail++; }
    else       { printf("  ok    %s\n", what); }
}

/* Render `ms` of audio and return peak amplitude, 0..1. */
static float render_peak(plugin_api_v2_t *api, void *inst, int ms) {
    const int frames = 128;
    int blocks = (MOVE_SAMPLE_RATE * ms / 1000) / frames;
    int16_t buf[frames * 2];
    float peak = 0.0f;
    for (int b = 0; b < blocks; b++) {
        api->render_block(inst, buf, frames);
        for (int i = 0; i < frames * 2; i++) {
            float v = fabsf(buf[i] / 32768.0f);
            if (v > peak) peak = v;
        }
    }
    return peak;
}

static void note_on(plugin_api_v2_t *api, void *inst, int note, int vel) {
    uint8_t msg[3] = {0x90, (uint8_t)note, (uint8_t)vel};
    api->on_midi(inst, msg, 3, 0);
}
static void note_off(plugin_api_v2_t *api, void *inst, int note) {
    uint8_t msg[3] = {0x80, (uint8_t)note, 0};
    api->on_midi(inst, msg, 3, 0);
}

static std::string get(plugin_api_v2_t *api, void *inst, const char *key) {
    char buf[16384];
    int n = api->get_param(inst, key, buf, sizeof(buf));
    if (n < 0) return std::string("<err>");
    return std::string(buf);
}

int main(void) {
    plugin_api_v2_t *api = move_plugin_init_v2(NULL);
    ok(api != NULL && api->api_version == 2, "plugin exports api v2");

    void *inst = api->create_instance(NULL, NULL);
    ok(inst != NULL, "instance created");

    /* ---- silence ---- */
    ok(render_peak(api, inst, 100) < 0.001f, "idle output is silent");

    /* ---- a note makes a sound, and the string rings after release ---- */
    note_on(api, inst, 48, 100);
    float peak = render_peak(api, inst, 500);
    ok(peak > 0.02f, "C3 pluck produces audio");
    printf("        peak = %.3f\n", peak);

    note_off(api, inst, 48);
    float tail = render_peak(api, inst, 100);
    ok(tail > 0.0f, "string still rings just after release");
    float gone = render_peak(api, inst, 4000);
    ok(gone < tail, "and decays");

    /* ---- the playable window is enforced, keyswitches never sound ---- */
    api->set_param(inst, "all_notes_off", "1");
    render_peak(api, inst, 2000);
    note_on(api, inst, 47, 110);            /* one below C3 */
    ok(render_peak(api, inst, 200) < 0.001f, "B2 (below range) is ignored");
    note_off(api, inst, 47);
    note_on(api, inst, KS_MUTE, 110);
    ok(render_peak(api, inst, 200) < 0.001f, "the Mute keyswitch does not sound");
    note_off(api, inst, KS_MUTE);

    /* ---- a keyswitch and the latch drive the same Faust button ---- */
    chonk_t *c = (chonk_t *)inst;
    FAUSTFLOAT *zmute = c->bass_zones.find("Articulation_Mute");
    ok(zmute != NULL, "Articulation_Mute zone resolved");
    note_on(api, inst, KS_MUTE, 100);
    ok(*zmute >= 0.5f, "held keyswitch turns Mute on");
    note_off(api, inst, KS_MUTE);
    ok(*zmute < 0.5f, "releasing it turns Mute off");
    api->set_param(inst, "mute", "1");
    ok(*zmute >= 0.5f, "the latched param turns Mute on");
    /* The pad OVERRIDES the latch rather than adding to it: with Mute latched,
     * holding the pad lifts it for one note. */
    note_on(api, inst, KS_MUTE, 100);
    ok(*zmute < 0.5f, "holding the pad lifts a latched Mute");
    note_off(api, inst, KS_MUTE);
    ok(*zmute >= 0.5f, "releasing it restores the latch");
    ok(get(api, inst, "mute") == "1", "and the latch parameter itself never moved");
    api->set_param(inst, "mute", "0");
    ok(*zmute < 0.5f, "clearing the latch turns it off");

    /* ---- params reach the DSP, round-trip, and clamp ---- */
    api->set_param(inst, "bright", "80");
    FAUSTFLOAT *zb = c->bass_zones.find("Brightness");
    ok(zb && fabsf(*zb - 80.0f) < 0.01f, "brightness reaches the Faust zone");
    ok(get(api, inst, "bright") == "80.000", "and reads back");

    api->set_param(inst, "bright", "999");
    ok(get(api, inst, "bright") == "100.000", "over-range value clamps to max");
    api->set_param(inst, "gain", "-999");
    ok(get(api, inst, "gain") == "-60.000", "under-range value clamps to min");

    ok(api->get_param(inst, "no_such_param", (char *)alloca(64), 64) < 0,
       "an unknown key returns NEGATIVE, not 0");

    /* ---- EQ and Mix live in output.dsp, not bass.dsp ---- */
    api->set_param(inst, "eq3", "6");
    FAUSTFLOAT *zeq = c->out_zones.find("EQ_Band_3");
    ok(zeq && fabsf(*zeq - 6.0f) < 0.01f, "EQ band reaches the output DSP");

    /* ---- presets ---- */
    ok(CHONK_FACTORY_COUNT == 9, "all nine factory presets compiled in");
    ok(CHONK_FACTORY_PARAMS == CHONK_PARAM_COUNT,
       "the factory bank has one column per parameter");
    ok(std::string(CHONK_FACTORY[0].name) == "J-Bass", "slot 0 is J-Bass");

    api->set_param(inst, "preset", "3");
    ok(get(api, inst, "preset") == "3", "preset index round-trips");
    std::string pname = get(api, inst, "preset_name");
    ok(pname.size() > 0 && pname != "Init", "preset name is served");
    printf("        preset 3 = %s\n", pname.c_str());
    ok(get(api, inst, "preset_count") == "9", "preset_count is 9");

    /* A preset must actually move the sound, not just the index. */
    float pickup_before = c->value[(int)(find_param("pickup") - PARAMS)];
    api->set_param(inst, "preset", "1");   /* Defender: pickup 15.05 */
    float pickup_after = c->value[(int)(find_param("pickup") - PARAMS)];
    ok(fabsf(pickup_before - pickup_after) > 0.5f, "loading a preset moves parameters");

    /* Articulation state is performance state: a preset leaves it alone. */
    api->set_param(inst, "style", "Slap");
    api->set_param(inst, "preset", "5");
    ok(get(api, inst, "style") == "3", "a preset does not clear the playing style");
    api->set_param(inst, "style", "Off");

    /* ---- Style is ONE choice: the three Faust buttons are mutually exclusive.
     * params.lib folds them with max(), so two at once is a state the model
     * cannot represent — the enum is what stops the UI offering it. ---- */
    FAUSTFLOAT *zf = c->bass_zones.find("Articulation_Finger");
    FAUSTFLOAT *zp = c->bass_zones.find("Articulation_Pick");
    FAUSTFLOAT *zs = c->bass_zones.find("Articulation_Slap");
    ok(zf && zp && zs, "the three style zones resolve");
    api->set_param(inst, "style", "Pick");
    ok(*zp >= 0.5f && *zf < 0.5f && *zs < 0.5f, "Pick selects exactly one style");
    api->set_param(inst, "style", "2");            /* the same, as an index */
    ok(*zp >= 0.5f, "an enum index selects the same option");
    api->set_param(inst, "style", "Off");
    ok(*zf < 0.5f && *zp < 0.5f && *zs < 0.5f, "Off clears every style");

    /* A held style keyswitch wins while held, then hands back to the enum. */
    api->set_param(inst, "style", "Finger");
    note_on(api, inst, KS_SLAP, 100);
    ok(*zs >= 0.5f && *zf < 0.5f, "a held keyswitch overrides the enum");
    note_off(api, inst, KS_SLAP);
    ok(*zf >= 0.5f && *zs < 0.5f, "releasing it hands the string back to the enum");
    /* Rolling across two pads must not let the released one clear the held. */
    note_on(api, inst, KS_PICK, 100);
    note_on(api, inst, KS_SLAP, 100);
    note_off(api, inst, KS_PICK);
    ok(*zs >= 0.5f, "releasing a pad that is not the held one changes nothing");
    note_off(api, inst, KS_SLAP);
    api->set_param(inst, "style", "Off");

    /* ---- Ring: this port's release control ---- */
    FAUSTFLOAT *zring = c->bass_zones.find("Ring");
    ok(zring != NULL, "the Ring zone resolves");
    api->set_param(inst, "ring", "75");
    ok(fabsf(*zring - 75.0f) < 0.01f, "Ring reaches the DSP");
    ok(find_param("ring")->def == 0.0f, "Ring defaults to 0 — upstream's release");

    FAUSTFLOAT *zlet = c->bass_zones.find("Articulation_Ring");
    ok(zlet != NULL, "the Ring/Gate zone resolves");
    note_on(api, inst, KS_RING, 100);
    ok(*zlet >= 0.5f, "the G2 keyswitch rings the string out while held");
    note_off(api, inst, KS_RING);
    ok(*zlet < 0.5f, "and releases it");
    /* It is a PAD, not a latch: there is no parameter of that name, because
     * the Ring knob at 100 is the same value. */
    ok(find_param("let_ring") == NULL, "no latched Ring/Gate parameter exists");
    ok(api->get_param(inst, "let_ring", (char *)alloca(64), 64) < 0,
       "and asking for one returns negative");
    api->set_param(inst, "ring", "0");

    /* A released note must ring measurably longer with Ring up. Same note,
     * same velocity, same release point: only the knob differs. */
    {
        float tail[2];
        const float settings[2] = {0.0f, 100.0f};
        for (int k = 0; k < 2; k++) {
            void *t = api->create_instance(NULL, NULL);
            char v[8]; snprintf(v, sizeof(v), "%.0f", settings[k]);
            api->set_param(t, "ring", v);
            note_on(api, t, 48, 100);
            render_peak(api, t, 400);
            note_off(api, t, 48);
            render_peak(api, t, 150);          /* let the damping take hold */
            tail[k] = render_peak(api, t, 400);
            api->destroy_instance(t);
        }
        printf("        release tail: ring 0 = %.5f, ring 100 = %.5f\n", tail[0], tail[1]);
        ok(tail[1] > tail[0] * 2.0f, "Ring 100 leaves the string ringing after release");
    }

    /* ---- Glide and Frets ----
     * Upstream welded both to Legato. These assert the split by RENDERING a
     * note change and comparing the audio: a switch that reaches the DSP
     * changes the samples, and one that silently missed its zone does not.
     * (An earlier version of set_value returned before writing any toggle
     * without a keyswitch — which is exactly this bug, and it was invisible
     * from the parameter readback.) ---- */
    {
        FAUSTFLOAT *zg = c->bass_zones.find("Articulation_Glide");
        FAUSTFLOAT *zfr = c->bass_zones.find("Articulation_Frets");
        ok(zg && zfr, "the Glide and Frets zones resolve");
        api->set_param(inst, "glide", "1");
        ok(zg && *zg >= 0.5f, "Glide reaches the DSP");
        api->set_param(inst, "glide", "0");
        ok(zg && *zg < 0.5f, "and clears");
        ok(find_param("frets")->def == 1.0f, "Frets defaults ON (upstream's behaviour)");

        /* Render 48 -> 55 as an overlap, once per setting, and total the
         * absolute difference between the two renders. */
        auto render_change = [&](const char *key, const char *val, std::vector<int16_t> &out) {
            void *t = api->create_instance(NULL, NULL);
            api->set_param(t, key, val);
            note_on(api, t, 48, 100);
            int16_t buf[256];
            for (int b = 0; b < 140; b++) api->render_block(t, buf, 128);
            note_on(api, t, 55, 100);
            out.clear();
            for (int b = 0; b < 40; b++) {   /* ~115 ms, covers the 35 ms glide */
                api->render_block(t, buf, 128);
                out.insert(out.end(), buf, buf + 256);
            }
            api->destroy_instance(t);
        };
        auto diff = [](const std::vector<int16_t> &a, const std::vector<int16_t> &b) {
            double d = 0;
            for (size_t i = 0; i < a.size() && i < b.size(); i++) d += fabs((double)a[i] - b[i]);
            return d / (a.size() ? a.size() : 1);
        };
        std::vector<int16_t> off, on;
        render_change("glide", "0", off);
        render_change("glide", "1", on);
        double d_glide = diff(off, on);
        printf("        note change, mean sample diff: glide %.1f\n", d_glide);
        ok(d_glide > 10.0, "Glide alone changes how the pitch travels");

        /* Frets, with glide on in both renders: same journey, different path. */
        std::vector<int16_t> fret_on, fret_off;
        {
            void *t = api->create_instance(NULL, NULL);
            api->set_param(t, "glide", "1");
            api->set_param(t, "frets", "0");
            note_on(api, t, 48, 100);
            int16_t buf[256];
            for (int b = 0; b < 140; b++) api->render_block(t, buf, 128);
            note_on(api, t, 55, 100);
            for (int b = 0; b < 40; b++) {
                api->render_block(t, buf, 128);
                fret_off.insert(fret_off.end(), buf, buf + 256);
            }
            api->destroy_instance(t);
        }
        fret_on = on;   /* glide 1, frets at its default of ON */
        double d_frets = diff(fret_on, fret_off);
        printf("        glided change, mean sample diff: frets %.1f\n", d_frets);
        ok(d_frets > 10.0, "Frets changes the path of a glide");

        /* Glide Time: the same glide, taken slowly, must differ from the
         * default 35 ms — and with Glide OFF the time must do nothing at all,
         * because the DSP multiplies it by glideTerm. */
        auto render_glide = [&](const char *glide, const char *ms, std::vector<int16_t> &out) {
            void *t = api->create_instance(NULL, NULL);
            api->set_param(t, "glide", glide);
            api->set_param(t, "glide_ms", ms);
            note_on(api, t, 48, 100);
            int16_t buf[256];
            for (int b = 0; b < 140; b++) api->render_block(t, buf, 128);
            note_on(api, t, 55, 100);
            out.clear();
            for (int b = 0; b < 60; b++) {
                api->render_block(t, buf, 128);
                out.insert(out.end(), buf, buf + 256);
            }
            api->destroy_instance(t);
        };
        std::vector<int16_t> fast, slow, jump_a, jump_b;
        render_glide("1", "35", fast);
        render_glide("1", "400", slow);
        render_glide("0", "35", jump_a);
        render_glide("0", "400", jump_b);
        double d_time = diff(fast, slow), d_off = diff(jump_a, jump_b);
        printf("        glide time 35 vs 400 ms: %.1f (glide on), %.1f (glide off)\n",
               d_time, d_off);
        ok(d_time > 10.0, "Glide Time changes how long the glide takes");
        ok(d_off == 0.0, "and does nothing at all with Glide off");
        ok(find_param("glide_ms")->def == 35.0f, "Glide Time defaults to upstream's 35 ms");
    }

    /* ---- Retrigger ----
     * Faust fires the pluck on a RISING edge of Trigger, so a second note at
     * the SAME velocity while the first is held does not re-pluck. Retrigger
     * forces the edge by holding the zone at 0 for one rendered frame. ---- */
    {
        float attack[2];
        for (int k = 0; k < 2; k++) {
            void *t = api->create_instance(NULL, NULL);
            api->set_param(t, "retrig", k ? "1" : "0");
            note_on(api, t, 48, 100);
            render_peak(api, t, 400);              /* let the first note settle */
            /* Same note, same velocity: the case the edge detector eats. */
            note_on(api, t, 48, 100);
            attack[k] = render_peak(api, t, 40);
            api->destroy_instance(t);
        }
        printf("        same-velocity restrike: off = %.4f, retrig = %.4f\n",
               attack[0], attack[1]);
        ok(attack[1] > attack[0] * 1.2f, "Retrigger re-plucks a same-velocity restrike");
    }

    /* The forced edge must not cost a frame of audio: the block still renders
     * exactly the frames it was asked for, and none of them are dropped. */
    {
        void *t = api->create_instance(NULL, NULL);
        api->set_param(t, "retrig", "1");
        note_on(api, t, 48, 100);
        int16_t buf[128 * 2];
        for (int i = 0; i < 128 * 2; i++) buf[i] = 0x7777;   /* poison */
        api->render_block(t, buf, 128);
        int untouched = 0;
        for (int i = 0; i < 128 * 2; i++) if (buf[i] == 0x7777) untouched++;
        ok(untouched == 0, "a forced retrigger still fills every frame of the block");
        chonk_t *tc = (chonk_t *)t;
        ok(tc->pending_trigger < 0.0f, "and the pending edge is consumed by that block");
        api->destroy_instance(t);
    }

    /* Off by default: upstream's behaviour is what an untouched patch gets. */
    ok(find_param("retrig")->def == 0.0f, "Retrigger defaults to off");

    /* ---- state blob ---- */
    api->set_param(inst, "tone", "17");
    api->set_param(inst, "octave_transpose", "-1");
    std::string state = get(api, inst, "state");
    ok(state.find("\"tone\":17.0000") != std::string::npos, "state carries edited values");

    void *inst2 = api->create_instance(NULL, state.c_str());
    ok(inst2 != NULL, "a second instance restores from a state blob");
    ok(get(api, inst2, "tone") == "17.000", "restored value wins over the preset's");
    ok(get(api, inst2, "octave_transpose") == "-1", "octave transpose restored");
    api->destroy_instance(inst2);

    /* ---- the UI surface ---- */
    std::string uih = get(api, inst, "ui_hierarchy");
    ok(uih.find("\"root\"") != std::string::npos, "ui_hierarchy is served");
    std::string cp = get(api, inst, "chain_params");
    ok(cp.size() > 100 && cp[0] == '[', "chain_params is served");
    for (int i = 0; i < CHONK_PARAM_COUNT; i++) {
        std::string want = std::string("\"key\":\"") + PARAMS[i].key + "\"";
        if (cp.find(want) == std::string::npos) {
            printf("  FAIL  chain_params is missing %s\n", PARAMS[i].key);
            g_fail++;
        }
        g_checks++;
    }
    printf("  ok    chain_params declares every parameter\n");
    /* Percent params must declare max, or the shared formatter reads 50 as 5000%. */
    ok(cp.find("\"unit\":\"%\"") != std::string::npos &&
       cp.find("\"max\":100,\"default\":50,\"unit\":\"%\"") != std::string::npos,
       "percent params carry an explicit max");

    /* Every key named in the hierarchy must be a real param. */
    size_t pos = 0;
    int checked = 0;
    while ((pos = uih.find('"', pos)) != std::string::npos) {
        size_t end = uih.find('"', pos + 1);
        if (end == std::string::npos) break;
        std::string tok = uih.substr(pos + 1, end - pos - 1);
        pos = end + 1;
        /* Only the knob/param arrays hold bare keys; skip structural words. */
        if (tok == "modes" || tok == "levels" || tok == "label" || tok == "name" ||
            tok == "knobs" || tok == "params" || tok == "level" ||
            tok == "list_param" || tok == "count_param" || tok == "name_param" ||
            tok == "preset" || tok == "preset_count" || tok == "preset_name") continue;
        if (find_param(tok.c_str())) { checked++; continue; }
        /* level names and their labels are not params */
        if (uih.find("\"" + tok + "\":{") != std::string::npos) continue;
        if (tok.find(' ') != std::string::npos || tok == "CHONK" ||
            tok == "String" || tok == "Articulation" || tok == "EQ" ||
            tok == "Mix" || tok == "MIDI") continue;
        printf("  FAIL  ui_hierarchy names unknown key '%s'\n", tok.c_str());
        g_fail++;
    }
    g_checks++;
    ok(checked > 20, "ui_hierarchy keys all resolve to parameters");

    api->destroy_instance(inst);

    printf("\n%d checks, %d failed\n", g_checks, g_fail);
    return g_fail ? 1 : 0;
}
