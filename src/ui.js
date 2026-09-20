/*
 * CHONK UI for Schwung / Ableton Move
 *
 * Physically modeled electric bass, ported from OneTrick CHONK (Punk Labs LLC).
 * The root screen is the shared sound-generator surface (preset browser +
 * octave); the parameter tree itself is declared by the plugin's ui_hierarchy
 * and drawn by the Shadow UI.
 *
 * GPL-3.0-or-later (DSP: Punk Labs LLC)
 */

import { createSoundGeneratorUI } from '/data/UserData/schwung/shared/sound_generator_ui.mjs';

const ui = createSoundGeneratorUI({
    moduleName: 'CHONK',

    onOctaveChange: () => {
        /* One string, and the octave shift moves the articulation keyswitches
         * with the notes — drop everything held rather than leave the string
         * gated at a pitch no pad can release. */
        host_module_set_param('all_notes_off', '1');
    },

    /* Monophonic by design: one waveguide, one string. */
    showPolyphony: true,
    polyphony: 1,
    showOctave: true,
});

globalThis.init                  = ui.init;
globalThis.tick                  = ui.tick;
globalThis.onMidiMessageInternal = ui.onMidiMessageInternal;
globalThis.onMidiMessageExternal = ui.onMidiMessageExternal;
