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
    note_on(api, inst, KS_MUTE, 100);
    note_off(api, inst, KS_MUTE);
    ok(*zmute >= 0.5f, "a keyswitch press does not clear the latch");
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

    /* Articulation latches are performance state: a preset leaves them alone. */
    api->set_param(inst, "slap", "1");
    api->set_param(inst, "preset", "5");
    ok(get(api, inst, "slap") == "1", "a preset does not clear articulation latches");
    api->set_param(inst, "slap", "0");

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
