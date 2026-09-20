/*
   ┏━━━┓╱╱╱╱╱┏┓╱╱┏┓╱╱╱╱╱┏┓╱╱
   ┃┏━┓┃╱╱╱╱╱┃┃╱╱┃┃╱╱╱╱╱┃┃╱╱╱
   ┃┗━┛┣┓┏┳━━┫┃┏┓┃┃╱╱┏━━┫┗━┳━━┓
   ┃┏━━┫┃┃┃┏┓┃┗┛┃┃┃╱┏┫┏┓┃┏┓┃━━┫
   ┃┃╱╱┃┗┛┃┃┃┃┏┓┃┃┗━┛┃┏┓┃┗┛┣━━┃
   ┗┛╱╱┗━━┻┛┗┻┛┗┛┗━━━┻┛┗┻━━┻━━┛
    ━━━━━━━━━━━━━━━━━━━━━━━━━━

    Copyright (c) 2024 Punk Labs LLC

    This section is part of OneTrick CHONK

    OneTrick CHONK is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the Free
    Software Foundation, either version 3 of the License, or (at your option)
    any later version.

    OneTrick CHONK is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
    more details.

    You should have received a copy of the GNU General Public License along with
    OneTrick CHONK.  If not, see <http://www.gnu.org/licenses/>.
*/

use std::cell::RefCell;
//use std::rc::Rc;
use std::sync::atomic::{AtomicBool, Ordering};
use std::sync::Arc;

use nih_plug::prelude::*;

// #[cfg(feature = "egui")]
// use egui_extras::image::RetainedImage;

#[cfg(feature = "egui")]
use nih_plug_egui::{
    create_egui_editor,
    egui::{
        epaint::{
            Shadow,
        },
        style,
        Margin,
        //Align,
        Align2,
        Area,
        CentralPanel,
        Color32,
        Context,
        FontDefinitions,
        FontFamily,
        FontId,
        Frame,
        Id,
        //LayerId,
        //Layout,
        Order,
        pos2,
        Rect,
        Rounding,
        Sense,
        Stroke,
        vec2,
        Vec2,
    },
    EguiState,
};

mod dsp;
use onetrick::prelude::*;

//256 is 187.5Hz at 48kHz
//512 is 94Hz at 48kHz
//1024 is 47Hz at 48kHz
const MAX_BLOCK_SIZE: usize = 4096;
const NUM_CHANNELS: u32 = 2;
const MIN_NOTE: u8 = MidiNote::C3 as u8;
const MAX_NOTE: u8 = MidiNote::G6 as u8;
const ARTICULATION_MUTE_NOTE: u8 = MidiNote::C2 as u8;
const ARTICULATION_FINGER_NOTE: u8 = MidiNote::D2 as u8;
const ARTICULATION_PICK_NOTE: u8 = MidiNote::E2 as u8;
const ARTICULATION_SLAP_NOTE: u8 = MidiNote::F2 as u8;
const ARTICULATION_SLIDEDOWN_NOTE: u8 = MidiNote::Cs2 as u8;
const ARTICULATION_SLIDEUP_NOTE: u8 = MidiNote::Ds2 as u8;
const ARTICULATION_LEGATO_NOTE: u8 = MidiNote::Fs2 as u8;


macro_rules! for_each_dsp {
    ($this:ident, $expression:expr) => {{
        $expression(&mut *$this.dsp_voice.borrow_mut().dsp_mut());
        $expression(&mut *$this.dsp_output.borrow_mut());
    }};
}

/// Main Struct for OneTrick Chonk
pub struct OneTrickChonk {
    params: Arc<OneTrickPluginParams>,

    accum_buffer: ResizableBuffer,

    dsp_output: RefCell<OneTrickDSP<dsp::modules::DSP_Output>>,
    dsp_voice: RefCell<OneTrickMonoVoice<dsp::modules::DSP_Bass>>,

    param_global_sensitivity: usize,
    //param_global_transpose: usize,

    param_articulation_mute: i32,
    param_articulation_finger: i32,
    param_articulation_pick: i32,
    param_articulation_slap: i32,
    param_articulation_slidedown: i32,
    param_articulation_slideup: i32,
    param_articulation_legato: i32,

    mute_indicator: Arc<AtomicBool>,
    finger_indicator: Arc<AtomicBool>,
    pick_indicator: Arc<AtomicBool>,
    slap_indicator: Arc<AtomicBool>,
    slideup_indicator: Arc<AtomicBool>,
    slidedown_indicator: Arc<AtomicBool>,
    legato_indicator: Arc<AtomicBool>,
    
    sample_rate: usize,
}

impl Default for OneTrickChonk {
    fn default() -> Self {
        let mut params = OneTrickPluginParams::default();
        #[cfg(feature = "egui")]
        {
            params.editor_state = EguiState::from_size(1135, 430);
        }

        let mut dsp_output = OneTrickDSP::new();
        params.append_dsp(&mut dsp_output, "");

        let mut dsp_voice = OneTrickDSP::new();
        params.append_dsp(&mut dsp_voice, "");

        let param_articulation_mute = dsp_voice.param_index("Articulation_Mute").unwrap();
        let param_articulation_finger = dsp_voice.param_index("Articulation_Finger").unwrap();
        let param_articulation_pick = dsp_voice.param_index("Articulation_Pick").unwrap();
        let param_articulation_slap = dsp_voice.param_index("Articulation_Slap").unwrap();
        let param_articulation_slidedown = dsp_voice.param_index("Articulation_SlideDown").unwrap();
        let param_articulation_slideup = dsp_voice.param_index("Articulation_SlideUp").unwrap();
        let param_articulation_legato = dsp_voice.param_index("Articulation_Legato").unwrap();
    
        
        let param_global_sensitivity = params.append_float(
            FloatParam::new(
                "MIDI Sensitivity",
                100.0,
                FloatRange::Linear {
                    min: 0.0,
                    max: 100.0,
                },
            )
            .with_unit("%")
            .with_value_to_string(Arc::new(|v| {
                format!("{value:.precision$}", precision = 1, value = v)
            })),
            "MIDI",
        );
        // let param_global_transpose = params.append_float(
        //     FloatParam::new(
        //         "MIDI Transpose",
        //         0.0,
        //         FloatRange::Linear {
        //             min: -12.0,
        //             max: 12.0,
        //         },
        //     )
        //     .with_unit("st")
        //     .with_value_to_string(Arc::new(|v| {
        //         format!("{value:.precision$}", precision = 1, value = v)
        //     })),
        //     "MIDI",
        // );

        let mute_indicator = Arc::new(AtomicBool::new(false));
        let finger_indicator = Arc::new(AtomicBool::new(false));
        let pick_indicator = Arc::new(AtomicBool::new(false));
        let slap_indicator = Arc::new(AtomicBool::new(false));
        let slideup_indicator = Arc::new(AtomicBool::new(false));
        let slidedown_indicator = Arc::new(AtomicBool::new(false));
        let legato_indicator = Arc::new(AtomicBool::new(false));

        Self {
            //params: Arc::new(OneTrickPluginParams::default()),
            params: Arc::new(params),

            accum_buffer: ResizableBuffer::default(),

            dsp_output: RefCell::new(dsp_output),
            dsp_voice: RefCell::new(OneTrickMonoVoice::new(dsp_voice)),

            param_global_sensitivity,
            //param_global_transpose,

            param_articulation_mute,
            param_articulation_finger,
            param_articulation_pick,
            param_articulation_slap,
            param_articulation_slidedown,
            param_articulation_slideup,
            param_articulation_legato,

            mute_indicator,
            finger_indicator,
            pick_indicator,
            slap_indicator,
            slideup_indicator,
            slidedown_indicator,
            legato_indicator,

            sample_rate: 1, // Invalid until initialized
        }
    }
}

impl OneTrickChonk {
    /// Set up fonts and default styles for Egui
    #[cfg(feature = "egui")]
    fn setup_egui_style(ctx: &Context) {
        // Start with the default fonts (we will be adding to them rather than replacing them).
        let mut fonts = FontDefinitions::default();

        BasicFonts::add_fonts(&mut fonts);
        Icons::add_fonts(&mut fonts);

        let mut style = style::Style::default();

        // Setup defaults for using generic controls:
        let font_id = FontId::new(17.0, FontFamily::Proportional);
        style.override_font_id = Some(font_id);
        //style.override_text_style = Some(TextStyle::Body);

        //style.widgets.noninteractive.bg_stroke = 
        //    Stroke::new(2.0, palette.grey().alpha(0.666).to_color32());

        // Set the base Style
        style.visuals = style::Visuals::dark();

        // Tooltips and Popup Windows:
        style.visuals.window_fill = Color32::DARK_GRAY;
        style.visuals.window_stroke = Stroke::default();
        style.visuals.popup_shadow = Shadow {offset: Vec2::new(0.0, 0.0), blur: 6.0, spread: 0.0, color: Color32::from_black_alpha(32)};
        style.visuals.override_text_color = Some(Color32::WHITE);
        
        style.visuals.selection.stroke = Stroke::new(2.0, Color32::WHITE);
        ctx.set_style(style);

        // Tell egui to use these fonts:
        ctx.set_fonts(fonts);
    }

    fn set_internal_params(&mut self) {
    }
}


impl Plugin for OneTrickChonk {
    const NAME: &'static str = "OneTrick CHONK";
    const VENDOR: &'static str = "Punk Labs";
    const URL: &'static str = env!("CARGO_PKG_HOMEPAGE");
    const EMAIL: &'static str = "contact@punklabs.com";

    const VERSION: &'static str = env!("CARGO_PKG_VERSION");

    const AUDIO_IO_LAYOUTS: &'static [AudioIOLayout] = &[
        AudioIOLayout {
            // Stereo
            main_input_channels: NonZeroU32::new(0),
            main_output_channels: NonZeroU32::new(NUM_CHANNELS),
            names: PortNames {
                layout: Some("Stereo"),

                main_input: None,
                main_output: Some("Output"),
                aux_inputs: &[],
                aux_outputs: &[],
            },
            ..AudioIOLayout::const_default()
        },
    ];

    const MIDI_INPUT: MidiConfig = MidiConfig::MidiCCs; //Basic or MidiCCs
    const MIDI_OUTPUT: MidiConfig = MidiConfig::None;

    const SAMPLE_ACCURATE_AUTOMATION: bool = false; // Too bad this doesn't split on Midi NoteOn events

    type SysExMessage = ();
    type BackgroundTask = ();

    fn params(&self) -> Arc<dyn Params> {
        self.params.clone()
    }

    fn initialize(
        &mut self,
        _audio_io_layout: &AudioIOLayout,
        buffer_config: &BufferConfig,
        _context: &mut impl InitContext<Self>,
    ) -> bool {
        // Resize buffers and perform other potentially expensive initialization operations here.
        // The `reset()` function is always called right after this function. You can remove this
        // function if you do not need it.

        let sample_rate = buffer_config.sample_rate as usize;

        self.accum_buffer.resize(2, MAX_BLOCK_SIZE);

        //nih_log!("Sample Rate: {}", sample_rate);
        self.sample_rate = sample_rate;
        const SILENCE_MS: u32 = 100;
        for_each_dsp!(self, &mut |dsp: &mut dyn OneTrickDSPGeneral| {
            dsp.initialize(sample_rate)
                .resize_buffer(MAX_BLOCK_SIZE)
                .track_silence(SILENCE_MS);
        });

        self.set_internal_params();

        true
    }

    fn reset(&mut self) {
        // Reset buffers and envelopes here. This can be called from the audio thread and may not
        // allocate. You can remove this function if you do not need it.
        // for_each_dsp!(self, &mut |dsp: &mut dyn OneTrickDSPGeneral| {
        //     dsp.reset();
        // });
        self.dsp_output.borrow_mut().reset();
        self.dsp_voice.borrow_mut().reset();

        self.set_internal_params();
    }

    fn process(
        &mut self,
        buffer: &mut Buffer,
        aux: &mut AuxiliaryBuffers,
        context: &mut impl ProcessContext<Self>,
    ) -> ProcessStatus {
        let _is_standalone = match context.plugin_api() {
            PluginApi::Standalone => true,
            _ => false,
        };

        ProcessEx::process_split_notes(
            buffer,
            aux,
            MAX_BLOCK_SIZE,
            context,
            |event| {
                // MIDI
                match event {
                    NoteEvent::MidiChannelPressure {
                        timing: _,
                        channel: _,
                        pressure,
                    } => {
                        self.dsp_voice.borrow_mut().aftertouch(pressure);
                    },
                    NoteEvent::MidiPitchBend {
                        timing: _,
                        channel: _,
                        value,
                    } => {
                        self.dsp_voice.borrow_mut().pitch_wheel(value);
                    }
                    NoteEvent::MidiCC {
                        timing: _,
                        channel: _,
                        cc,
                        value,
                    } => {
                        if let Some(cc) = MidiCC::try_from(cc).ok() {
                            match cc {
                                MidiCC::ModWheel => {
                                    self.dsp_voice.borrow_mut().mod_wheel(value);
                                    self.dsp_output.borrow_mut().mod_wheel(value);
                                },
                                MidiCC::Legato => {
                                    let toggle = if value > 0.5 {1.0} else {0.0};
                                    self.dsp_voice.borrow_mut().dsp_mut().set_param(self.param_articulation_legato, toggle);
                                    self.legato_indicator.store(toggle>0.5, Ordering::Relaxed);
                                }
                                _ => {}
                            }
                        }
                    }
                    NoteEvent::NoteOn {
                        timing: _,
                        voice_id: _,
                        channel: _,
                        note,
                        velocity,
                    } => {
                        // Piano range is A0->C8
                        // Safe range with treble dsp: A0->G6, Gs6 feels unstable
                        // Safe range without treble dsp: A0->C8
                        // Might have to disable treble dsp for now...
                        match note {
                            MIN_NOTE..=MAX_NOTE => {
                                //nih_log!("{}", note);
                                // Explodes at MidiNote::A6... (1760hz)

                                if self.dsp_voice.borrow().active_note_count() > 0 {
                                    self.dsp_voice.borrow_mut().dsp_mut().set_param(self.param_articulation_slidedown, 0.0);
                                    self.slidedown_indicator.store(false, Ordering::Relaxed);
                                    
                                    self.dsp_voice.borrow_mut().dsp_mut().set_param(self.param_articulation_slideup, 0.0);
                                    self.slideup_indicator.store(false, Ordering::Relaxed);
                                }

                                let velocity = if let Some(p) =
                                    self.params.param_float_at(self.param_global_sensitivity)
                                {
                                    let sensitivity = p.value() * 0.01;
                                    velocity * sensitivity + (1.0 - sensitivity) * (80.0 / 127.0)
                                } else {
                                    velocity
                                };
                                self.dsp_voice.borrow_mut().note_on(note, velocity);
                                self.dsp_output.borrow_mut().wake_up();
                            },
                            ARTICULATION_MUTE_NOTE => {
                                self.dsp_voice.borrow_mut().dsp_mut().set_param(self.param_articulation_mute, 1.0);
                                self.mute_indicator.store(true, Ordering::Relaxed);
                            }
                            ARTICULATION_FINGER_NOTE => {
                                self.dsp_voice.borrow_mut().dsp_mut().set_param(self.param_articulation_finger, 1.0);
                                self.finger_indicator.store(true, Ordering::Relaxed);
                            }
                            ARTICULATION_PICK_NOTE => {
                                self.dsp_voice.borrow_mut().dsp_mut().set_param
                                (self.param_articulation_pick, 1.0);
                                self.pick_indicator.store(true, Ordering::Relaxed);
                            }
                            ARTICULATION_SLAP_NOTE => {
                                self.dsp_voice.borrow_mut().dsp_mut().set_param(self.param_articulation_slap, 1.0);
                                self.slap_indicator.store(true, Ordering::Relaxed);
                            }
                            ARTICULATION_SLIDEDOWN_NOTE => {
                                self.dsp_voice.borrow_mut().dsp_mut().set_param(self.param_articulation_slidedown, velocity);
                                self.slidedown_indicator.store(true, Ordering::Relaxed);
                            }
                            ARTICULATION_SLIDEUP_NOTE => {
                                self.dsp_voice.borrow_mut().dsp_mut().set_param(self.param_articulation_slideup, velocity);
                                self.slideup_indicator.store(true, Ordering::Relaxed);
                            }
                            ARTICULATION_LEGATO_NOTE => {
                                self.dsp_voice.borrow_mut().dsp_mut().set_param(self.param_articulation_legato, 1.0);
                                self.legato_indicator.store(true, Ordering::Relaxed);
                            }
                            _ => {}
                        }
                    }
                    NoteEvent::NoteOff {
                        timing: _,
                        voice_id: _,
                        channel: _,
                        note,
                        velocity: _,
                    } => {
                        match note {
                            MIN_NOTE..=MAX_NOTE => {
                                self.dsp_voice.borrow_mut().note_off(note);

                                self.dsp_voice.borrow_mut().dsp_mut().set_param(self.param_articulation_slidedown, 0.0);
                                self.slidedown_indicator.store(false, Ordering::Relaxed);
                                
                                self.dsp_voice.borrow_mut().dsp_mut().set_param(self.param_articulation_slideup, 0.0);
                                self.slideup_indicator.store(false, Ordering::Relaxed);
                            }
                            ARTICULATION_MUTE_NOTE => {
                                self.dsp_voice.borrow_mut().dsp_mut().set_param(self.param_articulation_mute, 0.0);
                                self.mute_indicator.store(false, Ordering::Relaxed);
                            }
                            ARTICULATION_FINGER_NOTE => {
                                self.dsp_voice.borrow_mut().dsp_mut().set_param(self.param_articulation_finger, 0.0);
                                self.finger_indicator.store(false, Ordering::Relaxed);
                            }
                            ARTICULATION_PICK_NOTE => {
                                self.dsp_voice.borrow_mut().dsp_mut().set_param(self.param_articulation_pick, 0.0);
                                self.pick_indicator.store(false, Ordering::Relaxed);
                            }
                            ARTICULATION_SLAP_NOTE => {
                                self.dsp_voice.borrow_mut().dsp_mut().set_param(self.param_articulation_slap, 0.0);
                                self.slap_indicator.store(false, Ordering::Relaxed);
                            }
                            ARTICULATION_SLIDEDOWN_NOTE => {
                                self.dsp_voice.borrow_mut().dsp_mut().set_param(self.param_articulation_slidedown, 0.0);
                                self.slidedown_indicator.store(false, Ordering::Relaxed);
                            }
                            ARTICULATION_SLIDEUP_NOTE => {
                                self.dsp_voice.borrow_mut().dsp_mut().set_param(self.param_articulation_slideup, 0.0);
                                self.slideup_indicator.store(false, Ordering::Relaxed);
                            }
                            ARTICULATION_LEGATO_NOTE => {
                                self.dsp_voice.borrow_mut().dsp_mut().set_param(self.param_articulation_legato, 0.0);
                                self.legato_indicator.store(false, Ordering::Relaxed);
                            }
                            _ => {}
                        }
                    }
                    _ => (),
                }
            },
            #[allow(unused_variables)]
            &mut |buffer: &mut Buffer, aux: &mut AuxiliaryBuffers, block_start, block_end| // AUDIO
            {
                //let frames = output[0].len() as i32;
                let frames = block_end - block_start;

                // Attempt to process audio in blocks split by events:
                let output = &mut buffer.slice2ch_range_mut(block_start, block_end);
                for channel in output.iter_mut() {
                    channel.fill(0.0);
                }

                //self.accum_buffer.clear();
                self.accum_buffer.buffer.clear_frames(frames);

                // let pitch_shift = if let Some(p) =
                //     self.params.param_float_at(self.param_global_transpose)
                // {
                //     p.value()
                // } else {
                //     0.0
                // };

                {
                    let mut voice = self.dsp_voice.borrow_mut();
                    let dsp = voice.dsp_mut();
                    // Compute Voice:
                    if dsp.is_active() {
                        //dsp.transpose(pitch_shift);
                        dsp.compute(frames, &[]);
                    } else {
                        dsp.skip_compute();
                    }
                    // Accumulate Voice:
                    dsp.add_to_buffer(&mut self.accum_buffer.buffer);
                }
                // Process Main Output:
                {
                    let mut dsp = self.dsp_output.borrow_mut();
                    if dsp.is_active() {
                        dsp.compute_to(frames, &self.accum_buffer.buffer.as_slice_actually_immutable(), Some(output));
                    } else {
                        dsp.skip_compute();
                    }
                }

            },
        );

        ProcessStatus::Normal
    }

    #[cfg(feature = "egui")]
    fn editor(&mut self, _async_executor: AsyncExecutor<Self>) -> Option<Box<dyn Editor>> {
        //async_executor.execute_gui((||{nih_log!("Hello from the GUI")})());
        //async_executor.execute_background((||{nih_log!("Hello from a Background Thread")})());

        let params = self.params.clone();
        let mut preset_manager = PresetManager::new("OneTrick Chonk");
        let default_preset = Preset::from_param_defaults("P-Bass", &params);
        preset_manager.add_factory(default_preset);
        preset_manager.add_factory_string("J-Bass", include_str!("assets/presets/J-Bass.preset"));
        preset_manager.add_factory_string("Gargantuan", include_str!("assets/presets/Gargantuan.preset"));
        preset_manager.add_factory_string("Disco", include_str!("assets/presets/Disco.preset"));
        preset_manager.add_factory_string("Justice", include_str!("assets/presets/Justice.preset"));
        preset_manager.add_factory_string("Defender", include_str!("assets/presets/Defender.preset"));
        preset_manager.add_factory_string("Sitcom", include_str!("assets/presets/Sitcom.preset"));
        preset_manager.add_factory_string("Flatworm", include_str!("assets/presets/Flatworm.preset"));
        preset_manager.add_factory_string("Demonster", include_str!("assets/presets/Demonster.preset"));
        preset_manager.add_factory_string("New Strings", include_str!("assets/presets/New Strings.preset"));
       
        

        preset_manager.refresh();

        let palette = Palette::new(8)
            .shades(6)
            .white_level(0.95)
            .black_level(0.15)
            .saturation(0.65)
            .shift(0.49)
            .alt_hue_step(4.533)
            .dark_shift(0.1)
            .dark_desaturation(0.20);
        //let palette_alt = palette.alternate(1.5);

        let mut knob_style = ParamKnobStyle {
            radius: 35.0,
            indicator_style: ParamKnobIndicatorStyle::Continuous,
            ..Default::default()
        };
        let column_width = knob_style.required_width();

        let mut heading_style = LabelStyle::default_heading();
        heading_style.width = Some(column_width);
        heading_style.color = palette.black().into();
        heading_style.bg_color = Color32::TRANSPARENT;
        heading_style.shadow = None;
        let mut subheading_style = heading_style.clone();
        subheading_style.color = palette.black().into();
        subheading_style.bg_color = palette.color(5).into();
        subheading_style.shadow = None;



        let separator_style = SeparatorStyle {
            width: 6.0,
            color: palette.white().brightness(0.90).into(),
            ..Default::default()
        };
        knob_style.knob_color = palette.grey().into();
        knob_style.label_style.bg_color = palette.black().into();
        knob_style.label_style.color = palette.white().into();
        knob_style.label_style_hover.bg_color = palette.black().into();
        knob_style.label_style_hover.color = palette.color(0).into();
        knob_style.bg_color = palette.black().into();
        knob_style.indicator_fill_color = palette.grey().into();

        let mut knob_style_1 = knob_style.clone();
        knob_style_1.indicator_fill_color = palette.color_alt(0, 1).into();
        let mut knob_style_2 = knob_style.clone();
        knob_style_2.indicator_fill_color = palette.color(0).into();
        let mut knob_style_3 = knob_style.clone();
        knob_style_3.indicator_fill_color = palette.color(5).into();
        let mut knob_style_4 = knob_style.clone();
        knob_style_4.indicator_fill_color = palette.color(2).into();
        let mut knob_style_5 = knob_style.clone();
        knob_style_5.indicator_fill_color = palette.color(4).into();

        let mut slider_style = ParamSliderStyle {
            length: 70.0,
            ..Default::default()
        };
        /*
        slider_style.knob_aspect = 2.0/1.0;
        slider_style.knob_width = 20.0;
        slider_style.knob_round_ratio = 0.5;
        */
        slider_style.knob_color = knob_style.knob_color;
        slider_style.label_width = 70.0;
        slider_style.label_style.bg_color = palette.black().into();
        slider_style.label_style.color = knob_style.label_style.color;
        slider_style.label_style_hover.bg_color = knob_style.label_style_hover.bg_color;
        slider_style.label_style_hover.color = knob_style.label_style_hover.color;
        slider_style.bg_color = palette.black().into();
        slider_style.fill_color = Color32::LIGHT_GREEN;

        let mut slider_style_1 = slider_style.clone();
        slider_style_1.fill_color = knob_style_1.indicator_fill_color;
        let mut slider_style_2 = slider_style.clone();
        slider_style_2.fill_color = knob_style_2.indicator_fill_color;
        let mut slider_style_3 = slider_style.clone();
        slider_style_3.fill_color = knob_style_3.indicator_fill_color;
        let mut slider_style_4 = slider_style.clone();
        slider_style_4.fill_color = knob_style_4.indicator_fill_color;
        #[allow(clippy::redundant_clone)]
        let mut slider_style_5 = slider_style.clone();
        slider_style_5.fill_color = knob_style_5.indicator_fill_color;

        let show_credits = Arc::new(AtomicBool::new(false));
        //let anim_id = Id::new("local_anim"); // Can use in each context once
        let credits_anim_id = Id::new("credits_anim");
        //let piano_anim_id = Id::new("piano_anim");

        let mute_indicator = self.mute_indicator.clone();
        let finger_indicator = self.finger_indicator.clone();
        let pick_indicator = self.pick_indicator.clone();
        let slap_indicator = self.slap_indicator.clone();
        let slidedown_indicator = self.slidedown_indicator.clone();
        let slideup_indicator = self.slideup_indicator.clone();
        let legato_indicator = self.legato_indicator.clone();

        create_egui_editor(
            self.params.editor_state.clone(),
            preset_manager,
            move |ctx, _| {
                // DPI:
                // egui_baseview needs to be updated to support DPI.
                // We can adjust egui's rendering, but not egui_baseview's resolution.
                //ctx.set_pixels_per_point(2.0);

                //nih_log!("Editor::Open()");
                Self::setup_egui_style(ctx);
            },
            move |ctx, setter, state| {
                #[cfg(feature = "stress_test")] // Add some randomizing functionality
                {
                    // Stress regular automation (not multiple at once!)
                    Preset::from_random_values(&params)
                        .filter_params_include(&["Mix Gain"])
                        .apply_with_automation(&params, setter);

                    // Stress applying a preset
                    Preset::from_random_values(&params)
                        .apply(&params, setter);
                }

                //ctx.set_debug_on_hover(true);
                let preset_manager = state;

                //update()
                //let window_rect = ctx.available_rect();


                //let zoom_amount = 0.2;
                //let zoom_time = 0.150;
                //let indicator_raw = piano_indicator[0].load(Ordering::Relaxed);

                /*
                let _piano_indicator_zoom = 1.0
                    + ctx.animate_bool_with_time(
                        piano_anim_id,
                        indicator_raw,
                        if indicator_raw { 0.0 } else { zoom_time },
                    ) * zoom_amount;
                */
                let show_credits_amount =
                    ctx.animate_bool_with_time(credits_anim_id, show_credits.load(Ordering::Relaxed), 0.300);

                if show_credits_amount > 0.0 {
                    Area::new("credits_area".into())
                        .fixed_pos(pos2(0.0, 0.0))
                        .order(Order::Foreground)
                        .show(ctx, |ui| {
                            Frame::none()
                                .outer_margin(Margin::same(0.0))
                                .inner_margin(Margin::same(10.0))
                                .fill(Color32::from_black_alpha(
                                    (200.0 * show_credits_amount) as u8,
                                ))
                                //.rounding(panel_rounding)
                                .show(ui, |ui| {
                                    let available_size =
                                        vec2(ui.available_width(), ui.available_height());
                                    //let animated_offset = vec2(0.0, available_size.y * (1.0-show_credits_amount));
                                    ui.painter().text(
                                        (available_size * 0.5).to_pos2() + vec2(0.0, 15.0),
                                        Align2::CENTER_CENTER,
                                        format!(
                                            include_str!("CREDITS"),
                                            VERSION = env!("CARGO_PKG_VERSION")
                                        ),
                                        FontId::new(14.0, FontFamily::Name("Title".into())),
                                        Color32::from_white_alpha(
                                            (255.0 * show_credits_amount) as u8,
                                        ),
                                    );
                                    /*
                                    let galley = ui.painter().layout_no_wrap(
                                        include_str!("CREDITS").to_string(),
                                        FontId::new(20.0, FontFamily::Name("Title".into())),
                                        Color32::WHITE);
                                    let rect = Align2::CENTER_CENTER.anchor_rect(Rect::from_min_size((available_size * 0.5).to_pos2(), galley.size()));
                                    ui.painter().galley(rect.min, galley);
                                    */

                                    if ui
                                        .allocate_response(
                                            available_size,
                                            Sense::click()
                                            // Sense{click:true, drag:false, focusable:false},
                                        )
                                        .clicked()
                                    {
                                        show_credits.store(false, Ordering::Relaxed);
                                    }
                                });
                        });
                }
                CentralPanel::default()
                    .frame(
                        Frame::none()
                            .outer_margin(Margin::same(0.0))
                            .inner_margin(Margin::same(10.0))
                            //.fill(palette.black().into()),
                            .fill(palette.white().brightness(0.15).into()),
                    )
                    .show(ctx, |ui| {
                        // if let Some(focus) = ui.memory(|m|m.focus()) {
                        //     nih_log!("Focus: {}", focus.short_debug_format());
                        // }

                        //ctx.set_debug_on_hover(false);

                        let header_height = 60.0;
                        let neck_width = ui.available_width()+20.0;
                        let neck_color = palette.color(4).shade(3);
                        let trim_color = palette.color(5);//palette.white().brightness(0.9);

                        let neck_rect = Rect::from_min_size(pos2(0.,header_height+10.0), vec2(neck_width, 255.0));


                        //let lower_rect = Rect::from_min_size(pos2(0.0, 300.0), vec2(neck_width, 200.0));


                        //ui.painter().rect_filled(lower_rect, Rounding::ZERO, palette.white().brightness(0.20));

                        let eq_rect = Rect::from_center_size(pos2(387.5, 380.0), vec2(485.0, 300.0));
                        //ui.painter().parallelogram(eq_rect, -0.66, palette.white().alpha(0.1).into());
                        ui.painter().rect_filled(eq_rect, Rounding::same(1000.0), trim_color.clone());//.brightness(0.9));


                        ui.painter().rect_filled(neck_rect, Rounding::ZERO, neck_color);
                        ui.painter().rect_stroke(neck_rect.expand2(vec2(4.0, -2.0)), Rounding::ZERO, Stroke::new(4.0, trim_color.clone()));


                        // ui.painter().rect_filled(
                        //     Rect::from_min_max(pos2(0.0, 0.0), pos2(415.0, header_height)),
                        //     Rounding::ZERO,
                        //     palette.white().brightness(0.05).to_color32(),
                        // );

       
                        // ui.painter().parallelograms(
                        //     Rect::from_min_size(pos2(325.0, 0.0), vec2(220.0, header_height)),
                        //     1.0,
                        //     &[
                        //         //palette.color(0).shade(1).to_color32(),                                
                        //         palette.white().to_color32(),
                        //         palette.black().to_color32(),
                        //         palette.white().to_color32(),
                        //         palette.black().to_color32(),
                        //         palette.white().to_color32(),
                        //         //palette.color(0).shade(1).to_color32(),
                        //     ],
                        // );
                        // let hack_height = header_height*0.63;
                        // ui.painter().parallelograms(
                        //     Rect::from_min_size(pos2(325.0, hack_height), vec2(220.0-hack_height, header_height-hack_height)),
                        //     1.0,
                        //     &[
                        //         palette.white().to_color32(),
                        //     ],
                        // );


                        ui.set_width(ui.available_width());

                        //let panel_rounding = Rounding::same(8.0);
                        let panel_rounding = Rounding {
                            nw: 8.0,
                            ne: 0.0,
                            sw: 8.0,
                            se: 0.0,
                        };
                        //let panel_shadow = Shadow::small_light();
                        let panel_shadow = Shadow {offset: Vec2::new(0.0, 0.0), blur: 24.0, spread: 0.0, color: Color32::from_black_alpha(24)};

                        ui.horizontal(|ui| {
                            // =========================================
                            // ============== MAIN PANEL ==============
                            // =========================================
                            Frame::none()
                                .outer_margin(Margin::same(0.0))
                                .inner_margin(Margin::symmetric(10.0, 10.0))
                                .fill(Color32::TRANSPARENT)
                                //.stroke(Stroke::new(1.0, palette.black()))
                                .show(ui, |ui| {
                                ui.vertical(|ui| { // MAIN
                                    ui.set_width(75.0*8.0-60.0);
                                    ui.set_height(660.0);
                                    ui.add_space(header_height);
                                    /*
                                    ui.vertical_centered(|ui| {
                                        ui.add(
                                            Label::new("Kit").with_style(&heading_style),
                                        );
                                    });
                                    ui.add_space(5.0);
                                    */
                                    let center_offset_x = 45.0;
                                    let spacing_y = 15.0;
                                    ui.horizontal(|ui| {
                                        let justify_spacing_x = 20.0; //22.0;
                                        ui.spacing_mut().item_spacing = vec2(justify_spacing_x, spacing_y);

                                        ui.set_height(232.0);
                                        ui.vertical(|ui| {
                                            ui.horizontal(|ui| {
                                                ui.add_space(center_offset_x);
                                                ui.add(Label::new("Mix").with_style(&subheading_style));
                                            });
                                            ui.horizontal(|ui| {
                                                ui.add(
                                                    ParamKnob::for_param(
                                                        params.param_float("Mix Gain"),
                                                        setter,
                                                    )
                                                    .with_label("Gain")
                                                    .with_style(&knob_style_5),
                                                );
                                                ui.add(
                                                    ParamKnob::for_param(
                                                        params.param_float("Mix Saturation"),
                                                        setter,
                                                    )
                                                    .with_label("Saturation")
                                                    .with_style(&knob_style_5),
                                                );                                            
                                            });
                                            ui.horizontal(|ui| {
                                                ui.add_space(10.0);
                                                ui.add(
                                                    ParamSlider::for_param(
                                                        params.param_float("Mix Pan"),
                                                        setter,
                                                    )
                                                    .with_label("Pan")
                                                    .with_style(&slider_style_5)
                                                    .with_indicator_from_center()
                                                    .with_orientation(ParamSliderOrientation::Horizontal)
                                                    .with_length(140.0)
                                                    .with_label_width(70.0),
                                                );
                                            });
                                        });
                                        ui.add(Separator::new().with_style(&separator_style));
                                        ui.vertical(|ui| {
                                            ui.horizontal(|ui| {
                                                ui.add_space(center_offset_x*2.5);
                                                ui.add(Label::new("Bass").with_style(&subheading_style));
                                            });
                                            ui.horizontal(|ui| {
                                                ui.add_space(center_offset_x);
                                                ui.add(
                                                    ParamKnob::for_param(
                                                        params.param_float("Bass Pickup Position"),
                                                        setter,
                                                    )
                                                    .with_label("Pickups")
                                                    .with_style(&knob_style_3),
                                                );
                                                ui.add(
                                                    ParamKnob::for_param(
                                                        params.param_float("Bass Sustain"),
                                                        setter,
                                                    )
                                                    .with_label("Sustain")
                                                    .with_style(&knob_style_3),
                                                );
                                                ui.add(
                                                    ParamKnob::for_param(
                                                        params.param_float("Bass String Brightness"),
                                                        setter,
                                                    )
                                                    .with_label("Strings")
                                                    .with_style(&knob_style_4),
                                                );
                                            });
                                            ui.horizontal(|ui| {
                                                ui.add(
                                                    ParamKnob::for_param(
                                                        params.param_float("Bass Strike Hardness"),
                                                        setter,
                                                    )
                                                    .with_label("Strike")
                                                    .with_style(&knob_style_2),
                                                );
                                                ui.add(
                                                    ParamKnob::for_param(
                                                        params.param_float("Bass Thump"),
                                                        setter,
                                                    )
                                                    .with_label("Thump")
                                                    .with_style(&knob_style_2),
                                                );
                                                ui.add(
                                                    ParamKnob::for_param(
                                                        params.param_float("Bass Tone Knob"),
                                                        setter,
                                                    )
                                                    .with_label("Tone")
                                                    .with_style(&knob_style_4),
                                                );
                                                });
                                        });
                                        let justify_spacing_x = 20.0; //22.0;
                                        ui.spacing_mut().item_spacing = vec2(justify_spacing_x, spacing_y);

                                        ui.add(Separator::new().with_style(&separator_style));

                                        ui.vertical(|ui| {
                                            ui.horizontal(|ui| {
                                                ui.add_space(center_offset_x);
                                                ui.add(Label::new("MIDI").with_style(&subheading_style));
                                            });
                                            ui.horizontal(|ui|{
                                                //ui.add_space(center_offset_x);
                                                ui.add(
                                                    ParamKnob::for_param(
                                                        params.param_float("MIDI Sensitivity"),
                                                        setter,
                                                    )
                                                    .with_label("Sensitivity")
                                                    .with_style(&knob_style_5),
                                                );
                                                ui.add(
                                                    ParamKnob::for_param(
                                                        params.param_float("MIDI Fine Tune"),
                                                        setter,
                                                    )
                                                    .with_label("Tune")
                                                    .with_style(&knob_style_5),
                                                );
                                            });
                                            ui.horizontal(|ui|{
                                                ui.add(
                                                    ParamKnob::for_param(
                                                        params.param_float("MIDI Aftertouch Range"),
                                                        setter,
                                                    )
                                                    .with_label("Aftertouch")
                                                    .with_style(&knob_style_5)
                                                    .with_indicator_from_center(),
                                                );
                                                ui.add(
                                                    ParamKnob::for_param(
                                                        params.param_float("MIDI PitchWheel Range"),
                                                        setter,
                                                    )
                                                    .with_label("Pitch Wheel")
                                                    .with_style(&knob_style_5),
                                                );
                                            });
                                        });
                                    });
                                    ui.add_space(17.0);
                                    /*
                                    ui.horizontal(|ui| {
                                        ui.add_space(300.0);
                                        ui.add(Label::new("EQ").with_style(&subheading_style).with_color(palette.white().into()));
                                    });       
                                    ui.add_space(5.0);                             ui.
                                    */
                                    ui.horizontal(|ui| {
                                        ui.add_space(175.0);
                                        ui.horizontal(|ui|{
                                            ui.add(
                                                ParamSlider::for_param(
                                                    params.param_float("EQ Band 1"),
                                                    setter,
                                                )
                                                .with_label("100Hz")
                                                .with_style(&slider_style_5)
                                                .with_indicator_from_center(),
                                            );
                                            ui.add(
                                                ParamSlider::for_param(
                                                    params.param_float("EQ Band 2"),
                                                    setter,
                                                )
                                                .with_label("250Hz")
                                                .with_style(&slider_style_5)
                                                .with_indicator_from_center(),
                                            );
                                            ui.add(
                                                ParamSlider::for_param(
                                                    params.param_float("EQ Band 3"),
                                                    setter,
                                                )
                                                .with_label("500Hz")
                                                .with_style(&slider_style_5)
                                                .with_indicator_from_center(),
                                            );
                                            ui.add(
                                                ParamSlider::for_param(
                                                    params.param_float("EQ Band 4"),
                                                    setter,
                                                )
                                                .with_label("1.5kHz")
                                                .with_style(&slider_style_5)
                                                .with_indicator_from_center(),
                                            );
                                            ui.add(
                                                ParamSlider::for_param(
                                                    params.param_float("EQ Band 5"),
                                                    setter,
                                                )
                                                .with_label("3kHz")
                                                .with_style(&slider_style_5)
                                                .with_indicator_from_center(),
                                            );
                                        });
                                        ui.add_space(100.0);
                                        ui.vertical(|ui|{
                                            ui.add_space(72.0);
                                            ui.horizontal(|ui| {
                                                let off_style = LabelStyle {
                                                    bg_color: palette.white().brightness(0.3).into(),
                                                    color: palette.white().brightness(0.8).into(),
                                                    width: Some(55.0),
                                                    ..Default::default()
                                                };
                                                let on_style = LabelStyle {
                                                    bg_color: palette.color(0).into(),
                                                    color: palette.black().into(),
                                                    width: Some(55.0),
                                                    ..Default::default()
                                                };
                                                let styles = [off_style, on_style];
        
                                                let a_mute = mute_indicator.load(Ordering::Relaxed);
                                                let a_slap = slap_indicator.load(Ordering::Relaxed);
                                                let a_pick = pick_indicator.load(Ordering::Relaxed) && !a_slap;
                                                let a_finger = finger_indicator.load(Ordering::Relaxed) && !a_pick && !a_slap;
                                                let a_down = slidedown_indicator.load(Ordering::Relaxed);
                                                let a_up = slideup_indicator.load(Ordering::Relaxed);
                                                let a_legato = legato_indicator.load(Ordering::Relaxed);
        
                                                ui.add(Label::new("Mute").with_style(&styles[a_mute as usize]));
                                                ui.add(Label::new("Finger").with_style(&styles[a_finger as usize]));
                                                ui.add(Label::new("Pick").with_style(&styles[a_pick as usize]));
                                                ui.add(Label::new("Slap").with_style(&styles[a_slap as usize]));
                                                ui.add(Label::new("Down").with_style(&styles[a_down as usize]));
                                                ui.add(Label::new("Up").with_style(&styles[a_up as usize]));
                                                ui.add(Label::new("Legato").with_style(&styles[a_legato as usize]));                                            });
                                        })
                                    });
                                });
                            });
                            
                            //ui.add(Separator::new().with_style(&separator_style));

                            // ============================================
                            // ============== PRESETS PANELS ==============
                            // ============================================
                            let presets_height = 320.0;
                            ui.allocate_ui_at_rect(Rect::from_min_size(pos2(765.0, 5.0), vec2(360.0, presets_height)), |ui| {
                                Frame::none() // Global Panel Wrapper
                                    /*
                                    .outer_margin(Margin::same(0.0))
                                    .inner_margin(Margin::same(20.0))
                                    .rounding(panel_rounding)
                                    .fill(palette.white().into())
                                    .stroke(Stroke::new(2.0, palette.black()))
                                    */
                                    .show(ui, |ui| {

                                    let frame_outer_margin = Margin {
                                        top:5.0,
                                        ..Default::default()
                                    };
                                    //let frame_inner_margin = Margin::same(10.0);
                                    let frame_inner_margin = Margin{left: 0.0, right: 0.0, top: 5.0, bottom: 15.0};
                                    let subframe_inner_margin = Margin::same(10.0);
                                                                    
                                    //ui.set_max_width(75.0*5.0+30.0);
                                    ui.vertical_centered(|ui| {
                                        // ===========================================
                                        // ============== PRESETS PANEL ==============
                                        // ===========================================
                                        Frame::none()
                                            .outer_margin(frame_outer_margin)
                                            .inner_margin(frame_inner_margin)
                                            //.fill(palette.color(6).shade(1).into())
                                            .fill(palette.color(5).into())
                                            .rounding(panel_rounding)
                                            .shadow(panel_shadow)
                                            .show(ui, |ui| {

                                            ui.vertical_centered(|ui| {
                                                ui.add(
                                                    Label::new("Presets")
                                                    .with_style(&heading_style)
                                                );
                                            });

                                            Frame::none()
                                            .outer_margin(Margin::same(0.0))
                                            .inner_margin(subframe_inner_margin)
                                            //.fill(palette.color(6).shade(1).into())
                                            .fill(palette.color(4).shade(2).into())
                                            .show(ui, |ui| {
                                                // ============================================
                                                // ============== PRESETS PANEL ==============
                                                // ============================================
                                                //let preset_manager: PresetManager = preset_manager.borrow();
                                                let list_style = PresetListStyle {
                                                    bg_color: None,//Some(palette.dark_layer(1).into()),
                                                    color: palette.white().into(),
                                                    icon_style: IconButtonStyle{
                                                        color: palette.white().into(),
                                                        bg_color: Color32::TRANSPARENT,
                                                        color_hover: palette.white().into(),
                                                        bg_color_hover: palette.black().into(),
                                                        ..Default::default()
                                                    },
                                                    label_style: LabelStyle{
                                                        bg_color: palette.black().alpha(0.35).into(),
                                                        bg_color_hover: Some(palette.black().alpha(0.5).into()),
                                                        color: palette.white().into(),
                                                        color_hover: Some(palette.white().into()),
                                                        ..Default::default()
                                                    },
                                                    highlight_color: palette.color(0).into(),
                                                    highlight_bg_color: palette.black().alpha(0.9).into(),
                                                    search_bg_color: Some(palette.black().alpha(0.5).into()),
                                                    search_color: Some(palette.white().into()),
                                                    popup_offset_normalized: vec2(-1.0, 0.0),
                                                    ..Default::default()
                                                };

                                                ui.vertical_centered(|ui| {
                                                    ui.set_width(ui.available_width()+20.0);
                                                    //ui.set_height(ui.available_height()-10.0);
                                                    ui.set_height(presets_height);
                                                    PresetList::new()
                                                        .with_style(&list_style)
                                                        .show(ui, preset_manager, &params, setter);
                                                    /*
                                                    ListView::new([].iter(), ())
                                                        .title("Search".into())
                                                        .hold_text("something".into())
                                                        .striped()
                                                        .show(ctx, ui);
                                                    */
                                                });
                                            });
                                        });
                                    });
                                });
                            });
                        });

                        // Moved Logo down here to eliminate Area causing focus issues...
                        ui.allocate_ui_at_rect(Rect::from_min_size(pos2(0.0, 0.0), vec2(0.0, 0.0)), |ui| {
                            let response = ui.allocate_rect(
                                Rect::from_min_size(pos2(0.0, 0.0),
                                vec2(190.0, 70.0)),
                                // Sense::click());
                                Sense{click:true, drag:false, focusable:false});
                            if response.clicked() {
                                show_credits.store(true, Ordering::Relaxed);
                            }
                            ui.painter().one_trick_logo(
                                "CHONK",
                                pos2(15.0, 35.0),
                                52.0,
                                palette.color(5).into(),
                                if response.hovered() {palette.color(8).into()} else {palette.white().into()},
                            );
                        });

                    });
            },
        )
    }
}

impl ClapPlugin for OneTrickChonk {
    const CLAP_ID: &'static str = "com.punklabs.onetrick.chonk";
    const CLAP_DESCRIPTION: Option<&'static str> = Some("A Physically Modeled Bass Synth");
    const CLAP_MANUAL_URL: Option<&'static str> = Some(Self::URL);
    const CLAP_SUPPORT_URL: Option<&'static str> = None;
    const CLAP_FEATURES: &'static [ClapFeature] = &[
        ClapFeature::Instrument,  //Plugin Category
        ClapFeature::Synthesizer, // Plugin Sub-Category
        ClapFeature::Stereo,
    ]; // Audio Capabilities
       /*
       const CLAP_POLY_MODULATION_CONFIG: Option<PolyModulationConfig> = Some(PolyModulationConfig {
           // If the plugin's voice capacity changes at runtime (for instance, when switching to a
           // monophonic mode), then the plugin should inform the host in the `initialize()` function
           // as well as in the `process()` function if it changes at runtime using
           // `context.set_next_voice_capacity()`
           max_voice_capacity: MAX_VOICES as u32,
           // This enables voice stacking in Bitwig.
           supports_overlapping_voices: true,
       });
       */
}

impl Vst3Plugin for OneTrickChonk{
    const VST3_CLASS_ID: [u8; 16] = *b"OneTrick-CHONK  ";
    //const VST3_CATEGORIES: &'static str = "Instrument|Synth";
    const VST3_SUBCATEGORIES: &'static [Vst3SubCategory] =
        &[Vst3SubCategory::Instrument, Vst3SubCategory::Synth];
}

nih_export_clap!(OneTrickChonk);
nih_export_vst3!(OneTrickChonk);
