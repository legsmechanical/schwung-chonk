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

    /* 1 — J-Bass, a plain walking figure across the string's range. */
    g_api->set_param(g_inst, "preset", "0");
    render_ms(200);
    int walk[] = {48, 55, 52, 55, 50, 57, 53, 55};
    for (int i = 0; i < 8; i++) play(walk[i], 100, 280, 40);
    render_ms(600);

    /* 2 — the articulations, same note each time, so they can be compared. */
    play(60, 100, 500, 300);                       /* open           */
    g_api->set_param(g_inst, "mute", "1");
    play(60, 100, 500, 300);                       /* palm mute      */
    g_api->set_param(g_inst, "mute", "0");
    g_api->set_param(g_inst, "pick", "1");
    play(60, 100, 500, 300);                       /* pick           */
    g_api->set_param(g_inst, "pick", "0");
    g_api->set_param(g_inst, "slap", "1");
    play(60, 120, 500, 500);                       /* slap           */
    g_api->set_param(g_inst, "slap", "0");

    /* 3 — legato: hold a note and add another. The string re-pitches without
     *     re-plucking, which is the whole point of the mono voice. */
    midi3(0x90, 48, 100);
    render_ms(400);
    midi3(0x90, 51, 100);   /* takes the string over */
    render_ms(400);
    midi3(0x80, 51, 0);     /* hands it back to 48   */
    render_ms(600);
    midi3(0x80, 48, 0);
    render_ms(800);

    /* 4 — the slide keyswitch, bending up off a held note. */
    midi3(0x90, 50, 100);
    render_ms(300);
    midi3(0x90, KS_SLIDE_UP, 40);
    render_ms(500);
    midi3(0x80, KS_SLIDE_UP, 0);
    render_ms(400);
    midi3(0x80, 50, 0);
    render_ms(600);

    /* 5 — three more factory sounds on the same two notes. */
    for (int p = 2; p <= 4; p++) {
        char v[8]; snprintf(v, sizeof(v), "%d", p);
        g_api->set_param(g_inst, "preset", v);
        char name[64];
        g_api->get_param(g_inst, "preset_name", name, sizeof(name));
        printf("  preset %d: %s\n", p, name);
        play(48, 100, 400, 100);
        play(55, 110, 400, 500);
    }

    double cpu = (double)(clock() - t0) / CLOCKS_PER_SEC;
    double audio = g_pcm.size() / 2.0 / MOVE_SAMPLE_RATE;
    printf("rendered %.1f s of audio in %.2f s  (%.0fx realtime, workstation)\n",
           audio, cpu, audio / cpu);

    write_wav(out);
    g_api->destroy_instance(g_inst);
    return 0;
}
