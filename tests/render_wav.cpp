/*
 * Audition render: plays a short line through the module and writes a WAV,
 * so a change to the port can be HEARD without a device in front of you.
 *
 *   ./scripts/render.sh            -> build/chonk_demo.wav
 *
 * Also prints a realtime factor. That number is measured on the workstation,
 * NOT on Move's ARM core — it is useful for spotting a change that made the
 * DSP ten times more expensive, and useless as an absolute budget. The only
 * real CPU answer comes from the device.
 */
#include "../src/dsp/chonk_plugin.cpp"

#include <cstdio>
#include <ctime>
#include <vector>

static plugin_api_v2_t *g_api = NULL;
static void *g_inst = NULL;
static std::vector<int16_t> g_pcm;

static void render_ms(int ms) {
    const int frames = 128;
    int blocks = (MOVE_SAMPLE_RATE * ms / 1000) / frames;
    int16_t buf[frames * 2];
    for (int b = 0; b < blocks; b++) {
        g_api->render_block(g_inst, buf, frames);
        g_pcm.insert(g_pcm.end(), buf, buf + frames * 2);
    }
}

static void midi3(uint8_t a, uint8_t b, uint8_t c) {
    uint8_t m[3] = {a, b, c};
    g_api->on_midi(g_inst, m, 3, 0);
}

/* A note, held for `hold` ms, then `gap` ms of tail. */
static void play(int note, int vel, int hold, int gap) {
    midi3(0x90, (uint8_t)note, (uint8_t)vel);
    render_ms(hold);
    midi3(0x80, (uint8_t)note, 0);
    render_ms(gap);
}

static void write_wav(const char *path) {
    FILE *f = fopen(path, "wb");
    if (!f) { printf("cannot write %s\n", path); return; }
    uint32_t data_bytes = (uint32_t)(g_pcm.size() * 2);
    uint32_t rate = MOVE_SAMPLE_RATE, byte_rate = rate * 2 * 2;
    uint16_t ch = 2, bits = 16, fmt = 1, align = 4;
    uint32_t riff = 36 + data_bytes, fmt_size = 16;
    fwrite("RIFF", 1, 4, f); fwrite(&riff, 4, 1, f); fwrite("WAVE", 1, 4, f);
    fwrite("fmt ", 1, 4, f); fwrite(&fmt_size, 4, 1, f);
    fwrite(&fmt, 2, 1, f); fwrite(&ch, 2, 1, f); fwrite(&rate, 4, 1, f);
    fwrite(&byte_rate, 4, 1, f); fwrite(&align, 2, 1, f); fwrite(&bits, 2, 1, f);
    fwrite("data", 1, 4, f); fwrite(&data_bytes, 4, 1, f);
    fwrite(g_pcm.data(), 1, data_bytes, f);
    fclose(f);
    printf("wrote %s (%.1f s, %u bytes)\n", path, data_bytes / (float)byte_rate, data_bytes);
}

int main(int argc, char **argv) {
    const char *out = (argc > 1) ? argv[1] : "build/chonk_demo.wav";
    g_api = move_plugin_init_v2(NULL);
    g_inst = g_api->create_instance(NULL, NULL);

    clock_t t0 = clock();

    /* Everything below is an A/B: the SAME note, the same velocity, one
     * parameter moved. Announced in the console so the ear knows what it is
     * listening for. */
    auto set = [](const char *k, const char *v) { g_api->set_param(g_inst, k, v); };
    auto say = [](const char *what) { printf("  %s\n", what); };

    set("preset", "0");                     /* J-Bass, a plain starting point */
    render_ms(200);

    /* 1 — Strike Hardness at velocity 100, where the knob is actually in
     *     circuit. Subtle by upstream's design. */
    say("1. strike 0 vs 100, vel 100");
    set("strike", "0");   play(48, 100, 500, 400);
    set("strike", "100"); play(48, 100, 500, 600);

    /* 2 — the same at velocity 127, where articulationSlapAuto has taken the
     *     knob out of circuit entirely. These two should sound IDENTICAL. */
    say("2. strike 0 vs 100, vel 127 (auto-slap has bypassed the knob)");
    set("strike", "0");   play(48, 127, 500, 400);
    set("strike", "100"); play(48, 127, 500, 600);
    set("strike", "0");

    /* 3 — Thump, which moves the attack by about 4x in RMS. */
    say("3. thump 0 vs 100");
    set("thump", "0");   play(48, 100, 500, 400);
    set("thump", "100"); play(48, 100, 500, 600);

    /* 4 — the same Thump sweep with Mute engaged, where extraThump is pinned
     *     to 1.5 and the knob does nothing at all. */
    say("4. thump 0 vs 100 with Mute on (knob is out of circuit)");
    set("mute", "1");
    set("thump", "0");   play(48, 100, 500, 400);
    set("thump", "100"); play(48, 100, 500, 600);
    set("mute", "0");
    set("thump", "50");

    /* 5 — the playing styles, one knob now. */
    say("5. style: off / finger / pick / slap");
    const char *styles[] = {"Off", "Finger", "Pick", "Slap"};
    for (int i = 0; i < 4; i++) { set("style", styles[i]); play(48, 100, 500, 400); }
    set("style", "Off");
    render_ms(300);

    /* 6 — Ring: the release control this port adds. Same note, released at the
     *     same moment; only the knob differs. */
    say("6. ring 0 / 50 / 100 (note released after 400 ms)");
    const char *rings[] = {"0", "50", "100"};
    for (int i = 0; i < 3; i++) { set("ring", rings[i]); play(48, 100, 400, 1600); }
    set("ring", "0");

    /* 7 — Ring/Gate as a gesture: the G2 pad held across a phrase. */
    say("7. the Ring/Gate pad held across a phrase");
    midi3(0x90, KS_RING, 100);
    play(48, 100, 200, 100);
    play(55, 100, 200, 100);
    play(51, 100, 200, 1400);
    midi3(0x80, KS_RING, 0);
    render_ms(400);

    /* 8 — Retrigger: the same note at the same velocity, four times, while the
     *     first is still held. Off = one pluck ringing on; On = four. */
    say("8. same note x4 at one velocity, held: retrigger off, then on");
    for (int k = 0; k < 2; k++) {
        set("retrig", k ? "1" : "0");
        midi3(0x90, 48, 100);
        render_ms(300);
        for (int i = 0; i < 3; i++) { midi3(0x90, 48, 100); render_ms(300); }
        midi3(0x80, 48, 0);
        render_ms(700);
    }
    set("retrig", "0");

    /* 9 — Glide and Frets, on a held note change (48 -> 55 -> 48). */
    say("9. note change: no glide / glide fretted / glide smooth / legato");
    struct { const char *g, *f, *l; } gcase[] = {
        {"0","1","0"}, {"1","1","0"}, {"1","0","0"}, {"0","1","1"},
    };
    for (int i = 0; i < 4; i++) {
        set("glide", gcase[i].g); set("frets", gcase[i].f); set("legato", gcase[i].l);
        midi3(0x90, 48, 100); render_ms(400);
        midi3(0x90, 55, 100); render_ms(500);
        midi3(0x80, 55, 0);   render_ms(400);
        midi3(0x80, 48, 0);   render_ms(600);
    }
    set("glide", "0"); set("frets", "1"); set("legato", "0");

    double cpu = (double)(clock() - t0) / CLOCKS_PER_SEC;
    double audio = g_pcm.size() / 2.0 / MOVE_SAMPLE_RATE;
    printf("rendered %.1f s of audio in %.2f s  (%.0fx realtime, workstation)\n",
           audio, cpu, audio / cpu);

    write_wav(out);
    g_api->destroy_instance(g_inst);
    return 0;
}
