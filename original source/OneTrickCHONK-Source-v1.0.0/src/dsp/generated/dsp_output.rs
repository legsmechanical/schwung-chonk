/* ------------------------------------------------------------
copyright: "Copyright (c) 2024 Punk Labs LLC"
license: "GPLv3 (or later)"
name: "OneTrick CHONK Output"
Code generated with Faust 2.75.7 (https://faust.grame.fr)
Compilation options: -a arch.rs -lang rust -ct 0 -cn DSP_Output -dtl 65536 -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0
------------------------------------------------------------ */
use faust_types::*;
use onetrick::faust::FaustDspExtended;



fn DSP_Output_faustpower2_f(value: F32) -> F32 {
	return value * value;
}
mod ffi {
	use std::os::raw::{c_float};
	// Conditionally compile the link attribute only on non-Windows platforms
	#[cfg_attr(not(target_os="windows"), link(name="m"))]
	extern {
		pub fn remainderf(from: c_float, to: c_float) -> c_float;
		pub fn rintf(val: c_float) -> c_float;
	}
}
fn remainder_f32(from: f32, to: f32) -> f32 {
	unsafe { ffi::remainderf(from, to) }
}
fn rint_f32(val: f32) -> f32 {
	unsafe { ffi::rintf(val) }
}

#[cfg_attr(feature = "default-boxed", derive(default_boxed::DefaultBoxed))]
#[repr(C)]
pub struct DSP_Output {
	fSampleRate: i32,
	fConst0: F32,
	fConst1: F32,
	fConst2: F32,
	fConst3: F32,
	fConst4: F32,
	fConst5: F32,
	fConst6: F32,
	fConst7: F32,
	fButton0: F32,
	fVec0: [F32;2],
	fConst8: F32,
	fHslider0: F32,
	fRec2: [F32;2],
	fConst9: F32,
	fConst10: F32,
	fConst11: F32,
	fConst12: F32,
	fHslider1: F32,
	fRec4: [F32;2],
	fConst13: F32,
	fConst14: F32,
	fConst15: F32,
	fConst16: F32,
	fConst17: F32,
	fHslider2: F32,
	fRec6: [F32;2],
	fConst18: F32,
	fConst19: F32,
	fConst20: F32,
	fConst21: F32,
	fConst22: F32,
	fConst23: F32,
	fConst24: F32,
	fConst25: F32,
	fConst26: F32,
	fConst27: F32,
	fVec1: [F32;2],
	fConst28: F32,
	fConst29: F32,
	fRec9: [F32;2],
	fRec8: [F32;3],
	fHslider3: F32,
	fRec10: [F32;2],
	fRec12: [F32;2],
	fRec11: [F32;3],
	fRec7: [F32;3],
	fRec5: [F32;3],
	fRec3: [F32;3],
	fVec2: [F32;2],
	fConst30: F32,
	fConst31: F32,
	fRec1: [F32;2],
	fRec0: [F32;3],
	fHslider4: F32,
	fRec13: [F32;2],
	fRec15: [F32;2],
	fRec14: [F32;3],
	fHslider5: F32,
	fHslider6: F32,
	fRec16: [F32;2],
	fHslider7: F32,
	fRec17: [F32;2],
}

impl FaustDsp for DSP_Output {
	type T = F32;
		
	fn new() -> DSP_Output { 
		DSP_Output {
			fSampleRate: 0,
			fConst0: 0.0,
			fConst1: 0.0,
			fConst2: 0.0,
			fConst3: 0.0,
			fConst4: 0.0,
			fConst5: 0.0,
			fConst6: 0.0,
			fConst7: 0.0,
			fButton0: 0.0,
			fVec0: [0.0;2],
			fConst8: 0.0,
			fHslider0: 0.0,
			fRec2: [0.0;2],
			fConst9: 0.0,
			fConst10: 0.0,
			fConst11: 0.0,
			fConst12: 0.0,
			fHslider1: 0.0,
			fRec4: [0.0;2],
			fConst13: 0.0,
			fConst14: 0.0,
			fConst15: 0.0,
			fConst16: 0.0,
			fConst17: 0.0,
			fHslider2: 0.0,
			fRec6: [0.0;2],
			fConst18: 0.0,
			fConst19: 0.0,
			fConst20: 0.0,
			fConst21: 0.0,
			fConst22: 0.0,
			fConst23: 0.0,
			fConst24: 0.0,
			fConst25: 0.0,
			fConst26: 0.0,
			fConst27: 0.0,
			fVec1: [0.0;2],
			fConst28: 0.0,
			fConst29: 0.0,
			fRec9: [0.0;2],
			fRec8: [0.0;3],
			fHslider3: 0.0,
			fRec10: [0.0;2],
			fRec12: [0.0;2],
			fRec11: [0.0;3],
			fRec7: [0.0;3],
			fRec5: [0.0;3],
			fRec3: [0.0;3],
			fVec2: [0.0;2],
			fConst30: 0.0,
			fConst31: 0.0,
			fRec1: [0.0;2],
			fRec0: [0.0;3],
			fHslider4: 0.0,
			fRec13: [0.0;2],
			fRec15: [0.0;2],
			fRec14: [0.0;3],
			fHslider5: 0.0,
			fHslider6: 0.0,
			fRec16: [0.0;2],
			fHslider7: 0.0,
			fRec17: [0.0;2],
		}
	}
	fn metadata(&self, m: &mut dyn Meta) { 
		m.declare("analyzers.lib/name", r"Faust Analyzer Library");
		m.declare("analyzers.lib/version", r"1.2.0");
		m.declare("basics.lib/name", r"Faust Basic Element Library");
		m.declare("basics.lib/tabulateNd", r"Copyright (C) 2023 Bart Brouns <bart@magnetophon.nl>");
		m.declare("basics.lib/version", r"1.19.1");
		m.declare("compile_options", r"-a arch.rs -lang rust -ct 0 -cn DSP_Output -dtl 65536 -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0");
		m.declare("copyright", r"Copyright (c) 2024 Punk Labs LLC");
		m.declare("filename", r"output.dsp");
		m.declare("filters.lib/filterbank:author", r"Julius O. Smith III");
		m.declare("filters.lib/filterbank:copyright", r"Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m.declare("filters.lib/filterbank:license", r"MIT-style STK-4.3 license");
		m.declare("filters.lib/fir:author", r"Julius O. Smith III");
		m.declare("filters.lib/fir:copyright", r"Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m.declare("filters.lib/fir:license", r"MIT-style STK-4.3 license");
		m.declare("filters.lib/highpass:author", r"Julius O. Smith III");
		m.declare("filters.lib/highpass:copyright", r"Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m.declare("filters.lib/highshelf:author", r"Julius O. Smith III");
		m.declare("filters.lib/highshelf:copyright", r"Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m.declare("filters.lib/highshelf:license", r"MIT-style STK-4.3 license");
		m.declare("filters.lib/iir:author", r"Julius O. Smith III");
		m.declare("filters.lib/iir:copyright", r"Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m.declare("filters.lib/iir:license", r"MIT-style STK-4.3 license");
		m.declare("filters.lib/low_shelf:author", r"Julius O. Smith III");
		m.declare("filters.lib/low_shelf:copyright", r"Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m.declare("filters.lib/low_shelf:license", r"MIT-style STK-4.3 license");
		m.declare("filters.lib/lowpass0_highpass1", r"MIT-style STK-4.3 license");
		m.declare("filters.lib/lowpass0_highpass1:author", r"Julius O. Smith III");
		m.declare("filters.lib/lowpass:author", r"Julius O. Smith III");
		m.declare("filters.lib/lowpass:copyright", r"Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m.declare("filters.lib/lowpass:license", r"MIT-style STK-4.3 license");
		m.declare("filters.lib/lowshelf:author", r"Julius O. Smith III");
		m.declare("filters.lib/lowshelf:copyright", r"Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m.declare("filters.lib/lowshelf:license", r"MIT-style STK-4.3 license");
		m.declare("filters.lib/name", r"Faust Filters Library");
		m.declare("filters.lib/peak_eq:author", r"Julius O. Smith III");
		m.declare("filters.lib/peak_eq:copyright", r"Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m.declare("filters.lib/peak_eq:license", r"MIT-style STK-4.3 license");
		m.declare("filters.lib/peak_eq_cq:author", r"Julius O. Smith III");
		m.declare("filters.lib/peak_eq_cq:copyright", r"Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m.declare("filters.lib/peak_eq_cq:license", r"MIT-style STK-4.3 license");
		m.declare("filters.lib/tf1:author", r"Julius O. Smith III");
		m.declare("filters.lib/tf1:copyright", r"Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m.declare("filters.lib/tf1:license", r"MIT-style STK-4.3 license");
		m.declare("filters.lib/tf1s:author", r"Julius O. Smith III");
		m.declare("filters.lib/tf1s:copyright", r"Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m.declare("filters.lib/tf1s:license", r"MIT-style STK-4.3 license");
		m.declare("filters.lib/tf2:author", r"Julius O. Smith III");
		m.declare("filters.lib/tf2:copyright", r"Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m.declare("filters.lib/tf2:license", r"MIT-style STK-4.3 license");
		m.declare("filters.lib/tf2s:author", r"Julius O. Smith III");
		m.declare("filters.lib/tf2s:copyright", r"Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m.declare("filters.lib/tf2s:license", r"MIT-style STK-4.3 license");
		m.declare("filters.lib/version", r"1.3.0");
		m.declare("interpolators.lib/interpolate_linear:author", r"Stéphane Letz");
		m.declare("interpolators.lib/interpolate_linear:licence", r"MIT");
		m.declare("interpolators.lib/name", r"Faust Interpolator Library");
		m.declare("interpolators.lib/version", r"1.3.1");
		m.declare("license", r"GPLv3 (or later)");
		m.declare("maths.lib/author", r"GRAME");
		m.declare("maths.lib/copyright", r"GRAME");
		m.declare("maths.lib/license", r"LGPL with exception");
		m.declare("maths.lib/name", r"Faust Math Library");
		m.declare("maths.lib/version", r"2.8.0");
		m.declare("name", r"OneTrick CHONK Output");
		m.declare("onetrick.lib/copyright", r"Copyright (c) 2023 Punk Labs LLC");
		m.declare("onetrick.lib/license", r"GPLv3 (or later)");
		m.declare("onetrick.lib/name", r"OneTrick DSP Library");
		m.declare("platform.lib/name", r"Generic Platform Library");
		m.declare("platform.lib/version", r"1.3.0");
		m.declare("signals.lib/name", r"Faust Signal Routing Library");
		m.declare("signals.lib/version", r"1.6.0");
	}

	fn get_sample_rate(&self) -> i32 {
		return self.fSampleRate;
	}
	fn get_num_inputs(&self) -> i32 {
		return 1;
	}
	fn get_num_outputs(&self) -> i32 {
		return 2;
	}
	
	fn class_init(sample_rate: i32) {
	}
	fn instance_reset_params(&mut self) {
		self.fButton0 = 0.0;
		self.fHslider0 = 0.0;
		self.fHslider1 = 0.0;
		self.fHslider2 = 0.0;
		self.fHslider3 = 0.0;
		self.fHslider4 = 0.0;
		self.fHslider5 = 0.0;
		self.fHslider6 = 0.0;
		self.fHslider7 = 0.0;
	}
	fn instance_clear(&mut self) {
		for l0 in 0..2 {
			self.fVec0[l0 as usize] = 0.0;
		}
		for l1 in 0..2 {
			self.fRec2[l1 as usize] = 0.0;
		}
		for l2 in 0..2 {
			self.fRec4[l2 as usize] = 0.0;
		}
		for l3 in 0..2 {
			self.fRec6[l3 as usize] = 0.0;
		}
		for l4 in 0..2 {
			self.fVec1[l4 as usize] = 0.0;
		}
		for l5 in 0..2 {
			self.fRec9[l5 as usize] = 0.0;
		}
		for l6 in 0..3 {
			self.fRec8[l6 as usize] = 0.0;
		}
		for l7 in 0..2 {
			self.fRec10[l7 as usize] = 0.0;
		}
		for l8 in 0..2 {
			self.fRec12[l8 as usize] = 0.0;
		}
		for l9 in 0..3 {
			self.fRec11[l9 as usize] = 0.0;
		}
		for l10 in 0..3 {
			self.fRec7[l10 as usize] = 0.0;
		}
		for l11 in 0..3 {
			self.fRec5[l11 as usize] = 0.0;
		}
		for l12 in 0..3 {
			self.fRec3[l12 as usize] = 0.0;
		}
		for l13 in 0..2 {
			self.fVec2[l13 as usize] = 0.0;
		}
		for l14 in 0..2 {
			self.fRec1[l14 as usize] = 0.0;
		}
		for l15 in 0..3 {
			self.fRec0[l15 as usize] = 0.0;
		}
		for l16 in 0..2 {
			self.fRec13[l16 as usize] = 0.0;
		}
		for l17 in 0..2 {
			self.fRec15[l17 as usize] = 0.0;
		}
		for l18 in 0..3 {
			self.fRec14[l18 as usize] = 0.0;
		}
		for l19 in 0..2 {
			self.fRec16[l19 as usize] = 0.0;
		}
		for l20 in 0..2 {
			self.fRec17[l20 as usize] = 0.0;
		}
	}
	fn instance_constants(&mut self, sample_rate: i32) {
		self.fSampleRate = sample_rate;
		self.fConst0 = F32::min(1.92e+05, F32::max(1.0, (self.fSampleRate) as F32));
		self.fConst1 = 9424.778 / self.fConst0;
		self.fConst2 = F32::tan(self.fConst1);
		self.fConst3 = 1.0 / DSP_Output_faustpower2_f(self.fConst2);
		self.fConst4 = 2.0 * (1.0 - self.fConst3);
		self.fConst5 = 1.0 / self.fConst2;
		self.fConst6 = (self.fConst5 + -1.0) / self.fConst2 + 1.0;
		self.fConst7 = 1.0 / ((self.fConst5 + 1.0) / self.fConst2 + 1.0);
		self.fConst8 = F32::exp(-(1e+02 / self.fConst0));
		self.fConst9 = 6637.1675 / (self.fConst0 * F32::sin(self.fConst1));
		self.fConst10 = F32::tan(4712.389 / self.fConst0);
		self.fConst11 = 1.0 / self.fConst10;
		self.fConst12 = 2.0 * (1.0 - 1.0 / DSP_Output_faustpower2_f(self.fConst10));
		self.fConst13 = 2212.3892 / (self.fConst0 * F32::sin(3141.5928 / self.fConst0));
		self.fConst14 = 1570.7964 / self.fConst0;
		self.fConst15 = F32::tan(self.fConst14);
		self.fConst16 = 1.0 / self.fConst15;
		self.fConst17 = 2.0 * (1.0 - 1.0 / DSP_Output_faustpower2_f(self.fConst15));
		self.fConst18 = 1106.1946 / (self.fConst0 * F32::sin(self.fConst14));
		self.fConst19 = F32::tan(785.3982 / self.fConst0);
		self.fConst20 = 1.0 / self.fConst19;
		self.fConst21 = 2.0 * (1.0 - 1.0 / DSP_Output_faustpower2_f(self.fConst19));
		self.fConst22 = F32::tan(314.15927 / self.fConst0);
		self.fConst23 = 1.0 / DSP_Output_faustpower2_f(self.fConst22);
		self.fConst24 = 2.0 * (1.0 - self.fConst23);
		self.fConst25 = 1.0 / self.fConst22;
		self.fConst26 = (self.fConst25 + -1.0) / self.fConst22 + 1.0;
		self.fConst27 = 1.0 / ((self.fConst25 + 1.0) / self.fConst22 + 1.0);
		self.fConst28 = 1.0 - self.fConst25;
		self.fConst29 = 1.0 / (self.fConst25 + 1.0);
		self.fConst30 = 1.0 - self.fConst5;
		self.fConst31 = 1.0 / (self.fConst5 + 1.0);
	}
	fn instance_init(&mut self, sample_rate: i32) {
		self.instance_constants(sample_rate);
		self.instance_reset_params();
		self.instance_clear();
	}
	fn init(&mut self, sample_rate: i32) {
		DSP_Output::class_init(sample_rate);
		self.instance_init(sample_rate);
	}
	
	fn build_user_interface(&self, ui_interface: &mut dyn UI<Self::T>) {
		Self::build_user_interface_static(ui_interface);
	}
	
	fn build_user_interface_static(ui_interface: &mut dyn UI<Self::T>) {
		ui_interface.open_vertical_box("OneTrick CHONK Output");
		ui_interface.add_button("WakeUp", ParamIndex(0));
		ui_interface.declare(Some(ParamIndex(1)), "110", "");
		ui_interface.declare(Some(ParamIndex(1)), "export", "Gain");
		ui_interface.declare(Some(ParamIndex(1)), "group", "Mix");
		ui_interface.declare(Some(ParamIndex(1)), "unit", "dB");
		ui_interface.add_horizontal_slider("Mix_Gain", ParamIndex(1), 0.0, -1e+02, 6.0, 0.1);
		ui_interface.declare(Some(ParamIndex(2)), "120", "");
		ui_interface.declare(Some(ParamIndex(2)), "export", "Pan");
		ui_interface.declare(Some(ParamIndex(2)), "group", "Mix");
		ui_interface.declare(Some(ParamIndex(2)), "unit", "%");
		ui_interface.add_horizontal_slider("Mix_Pan", ParamIndex(2), 0.0, -1e+02, 1e+02, 0.01);
		ui_interface.declare(Some(ParamIndex(3)), "130", "");
		ui_interface.declare(Some(ParamIndex(3)), "export", "Saturation");
		ui_interface.declare(Some(ParamIndex(3)), "group", "Mix");
		ui_interface.declare(Some(ParamIndex(3)), "unit", "%");
		ui_interface.add_horizontal_slider("Mix_Saturation", ParamIndex(3), 0.0, 0.0, 1e+02, 0.01);
		ui_interface.declare(Some(ParamIndex(4)), "410", "");
		ui_interface.declare(Some(ParamIndex(4)), "export", "Band 1");
		ui_interface.declare(Some(ParamIndex(4)), "group", "EQ");
		ui_interface.declare(Some(ParamIndex(4)), "unit", "dB");
		ui_interface.add_horizontal_slider("EQ_Band_1", ParamIndex(4), 0.0, -12.0, 12.0, 0.01);
		ui_interface.declare(Some(ParamIndex(5)), "420", "");
		ui_interface.declare(Some(ParamIndex(5)), "export", "Band 2");
		ui_interface.declare(Some(ParamIndex(5)), "group", "EQ");
		ui_interface.declare(Some(ParamIndex(5)), "unit", "dB");
		ui_interface.add_horizontal_slider("EQ_Band_2", ParamIndex(5), 0.0, -12.0, 12.0, 0.01);
		ui_interface.declare(Some(ParamIndex(6)), "430", "");
		ui_interface.declare(Some(ParamIndex(6)), "export", "Band 3");
		ui_interface.declare(Some(ParamIndex(6)), "group", "EQ");
		ui_interface.declare(Some(ParamIndex(6)), "unit", "dB");
		ui_interface.add_horizontal_slider("EQ_Band_3", ParamIndex(6), 0.0, -12.0, 12.0, 0.01);
		ui_interface.declare(Some(ParamIndex(7)), "440", "");
		ui_interface.declare(Some(ParamIndex(7)), "export", "Band 4");
		ui_interface.declare(Some(ParamIndex(7)), "group", "EQ");
		ui_interface.declare(Some(ParamIndex(7)), "unit", "dB");
		ui_interface.add_horizontal_slider("EQ_Band_4", ParamIndex(7), 0.0, -12.0, 12.0, 0.01);
		ui_interface.declare(Some(ParamIndex(8)), "450", "");
		ui_interface.declare(Some(ParamIndex(8)), "export", "Band 5");
		ui_interface.declare(Some(ParamIndex(8)), "group", "EQ");
		ui_interface.declare(Some(ParamIndex(8)), "unit", "dB");
		ui_interface.add_horizontal_slider("EQ_Band_5", ParamIndex(8), 0.0, -12.0, 12.0, 0.01);
		ui_interface.close_box();
	}
	
	fn get_param(&self, param: ParamIndex) -> Option<Self::T> {
		match param.0 {
			0 => Some(self.fButton0),
			7 => Some(self.fHslider0),
			6 => Some(self.fHslider1),
			5 => Some(self.fHslider2),
			4 => Some(self.fHslider3),
			8 => Some(self.fHslider4),
			3 => Some(self.fHslider5),
			1 => Some(self.fHslider6),
			2 => Some(self.fHslider7),
			_ => None,
		}
	}
	
	fn set_param(&mut self, param: ParamIndex, value: Self::T) {
		match param.0 {
			0 => { self.fButton0 = value }
			7 => { self.fHslider0 = value }
			6 => { self.fHslider1 = value }
			5 => { self.fHslider2 = value }
			4 => { self.fHslider3 = value }
			8 => { self.fHslider4 = value }
			3 => { self.fHslider5 = value }
			1 => { self.fHslider6 = value }
			2 => { self.fHslider7 = value }
			_ => {}
		}
	}
	
	fn compute(&mut self, count: i32, inputs: &[&[Self::T]], outputs: &mut[&mut[Self::T]]) {
		let (inputs0) = if let [inputs0, ..] = inputs {
			let inputs0 = inputs0[..count as usize].iter();
			(inputs0)
		} else {
			panic!("wrong number of inputs");
		};
		let (outputs0, outputs1) = if let [outputs0, outputs1, ..] = outputs {
			let outputs0 = outputs0[..count as usize].iter_mut();
			let outputs1 = outputs1[..count as usize].iter_mut();
			(outputs0, outputs1)
		} else {
			panic!("wrong number of outputs");
		};
		let mut fSlow0: F32 = self.fButton0;
		let mut fSlow1: F32 = self.fHslider0;
		let mut fSlow2: F32 = self.fHslider1;
		let mut fSlow3: F32 = self.fHslider2;
		let mut fSlow4: F32 = self.fHslider3;
		let mut fSlow5: F32 = self.fHslider4;
		let mut fSlow6: F32 = self.fHslider5;
		let mut fSlow7: F32 = 1.0 / F32::min(F32::max(0.003 * fSlow6 + 0.5, 0.001), 0.999) + -2.0;
		let mut fSlow8: F32 = F32::max(-1e+02, self.fHslider6);
		let mut fSlow9: F32 = 0.01 * self.fHslider7;
		let mut fSlow10: F32 = 1.0 - 0.00525 * fSlow6;
		let zipped_iterators = inputs0.zip(outputs0).zip(outputs1);
		for ((input0, output0), output1) in zipped_iterators {
			self.fVec0[0] = fSlow0;
			let mut fTemp0: F32 = ((fSlow0 <= self.fVec0[1]) as i32) as u32 as F32;
			let mut fTemp1: F32 = 1.0 - self.fConst8 * fTemp0;
			self.fRec2[0] = fSlow1 * fTemp1 + self.fConst8 * fTemp0 * self.fRec2[1];
			let mut iTemp2: i32 = (self.fRec2[0] > 0.0) as i32;
			let mut fTemp3: F32 = self.fConst9 * F32::powf(1e+01, 0.05 * F32::abs(self.fRec2[0]));
			let mut fTemp4: F32 = if iTemp2 != 0 {self.fConst9} else {fTemp3};
			let mut fTemp5: F32 = self.fConst11 * (self.fConst11 + fTemp4) + 1.0;
			let mut fTemp6: F32 = if iTemp2 != 0 {fTemp3} else {self.fConst9};
			let mut fTemp7: F32 = self.fConst12 * self.fRec3[1];
			self.fRec4[0] = fSlow2 * fTemp1 + self.fConst8 * fTemp0 * self.fRec4[1];
			let mut iTemp8: i32 = (self.fRec4[0] > 0.0) as i32;
			let mut fTemp9: F32 = self.fConst13 * F32::powf(1e+01, 0.05 * F32::abs(self.fRec4[0]));
			let mut fTemp10: F32 = if iTemp8 != 0 {self.fConst13} else {fTemp9};
			let mut fTemp11: F32 = self.fConst16 * (self.fConst16 + fTemp10) + 1.0;
			let mut fTemp12: F32 = if iTemp8 != 0 {fTemp9} else {self.fConst13};
			let mut fTemp13: F32 = self.fConst17 * self.fRec5[1];
			self.fRec6[0] = fSlow3 * fTemp1 + self.fConst8 * fTemp0 * self.fRec6[1];
			let mut iTemp14: i32 = (self.fRec6[0] > 0.0) as i32;
			let mut fTemp15: F32 = self.fConst18 * F32::powf(1e+01, 0.05 * F32::abs(self.fRec6[0]));
			let mut fTemp16: F32 = if iTemp14 != 0 {self.fConst18} else {fTemp15};
			let mut fTemp17: F32 = self.fConst20 * (self.fConst20 + fTemp16) + 1.0;
			let mut fTemp18: F32 = if iTemp14 != 0 {fTemp15} else {self.fConst18};
			let mut fTemp19: F32 = self.fConst21 * self.fRec7[1];
			let mut fTemp20: F32 = *input0;
			self.fVec1[0] = fTemp20;
			self.fRec9[0] = -(self.fConst29 * (self.fConst28 * self.fRec9[1] - self.fConst25 * (fTemp20 - self.fVec1[1])));
			self.fRec8[0] = self.fRec9[0] - self.fConst27 * (self.fConst26 * self.fRec8[2] + self.fConst24 * self.fRec8[1]);
			self.fRec10[0] = fSlow4 * fTemp1 + self.fConst8 * fTemp0 * self.fRec10[1];
			self.fRec12[0] = -(self.fConst29 * (self.fConst28 * self.fRec12[1] - (fTemp20 + self.fVec1[1])));
			self.fRec11[0] = self.fRec12[0] - self.fConst27 * (self.fConst26 * self.fRec11[2] + self.fConst24 * self.fRec11[1]);
			self.fRec7[0] = self.fConst27 * ((self.fRec11[2] + self.fRec11[0] + 2.0 * self.fRec11[1]) * F32::powf(1e+01, 0.05 * self.fRec10[0]) + self.fConst23 * (self.fRec8[2] + (self.fRec8[0] - 2.0 * self.fRec8[1]))) - (self.fRec7[2] * (self.fConst20 * (self.fConst20 - fTemp16) + 1.0) + fTemp19) / fTemp17;
			self.fRec5[0] = (fTemp19 + self.fRec7[0] * (self.fConst20 * (self.fConst20 + fTemp18) + 1.0) + self.fRec7[2] * (self.fConst20 * (self.fConst20 - fTemp18) + 1.0)) / fTemp17 - (self.fRec5[2] * (self.fConst16 * (self.fConst16 - fTemp10) + 1.0) + fTemp13) / fTemp11;
			self.fRec3[0] = (fTemp13 + self.fRec5[0] * (self.fConst16 * (self.fConst16 + fTemp12) + 1.0) + self.fRec5[2] * (self.fConst16 * (self.fConst16 - fTemp12) + 1.0)) / fTemp11 - (self.fRec3[2] * (self.fConst11 * (self.fConst11 - fTemp4) + 1.0) + fTemp7) / fTemp5;
			let mut fTemp21: F32 = (fTemp7 + self.fRec3[0] * (self.fConst11 * (self.fConst11 + fTemp6) + 1.0) + self.fRec3[2] * (self.fConst11 * (self.fConst11 - fTemp6) + 1.0)) / fTemp5;
			self.fVec2[0] = fTemp21;
			self.fRec1[0] = -(self.fConst31 * (self.fConst30 * self.fRec1[1] - self.fConst5 * (fTemp21 - self.fVec2[1])));
			self.fRec0[0] = self.fRec1[0] - self.fConst7 * (self.fConst6 * self.fRec0[2] + self.fConst4 * self.fRec0[1]);
			self.fRec13[0] = fSlow5 * fTemp1 + self.fConst8 * fTemp0 * self.fRec13[1];
			self.fRec15[0] = -(self.fConst31 * (self.fConst30 * self.fRec15[1] - (fTemp21 + self.fVec2[1])));
			self.fRec14[0] = self.fRec15[0] - self.fConst7 * (self.fConst6 * self.fRec14[2] + self.fConst4 * self.fRec14[1]);
			let mut fTemp22: F32 = self.fConst7 * (self.fRec14[2] + self.fRec14[0] + 2.0 * self.fRec14[1] + self.fConst3 * F32::powf(1e+01, 0.05 * self.fRec13[0]) * (self.fRec0[0] + self.fRec0[2] - 2.0 * self.fRec0[1]));
			let mut fTemp23: F32 = F32::abs(fTemp22);
			let mut fTemp24: F32 = fSlow7 * (1.0 - fTemp23) + 1.0;
			let mut fTemp25: F32 = 1.0 - (i32::wrapping_mul(2, (fTemp22 < 0.0) as i32)) as F32;
			self.fRec16[0] = fSlow8 * fTemp1 + self.fConst8 * fTemp0 * self.fRec16[1];
			let mut fTemp26: F32 = F32::powf(1e+01, 0.05 * self.fRec16[0]);
			self.fRec17[0] = fSlow9 * fTemp1 + self.fConst8 * fTemp0 * self.fRec17[1];
			*output0 = fSlow10 * (F32::min(1.0, 1.0 - self.fRec17[0]) * fTemp26 * fTemp23 * fTemp25 / fTemp24);
			*output1 = fSlow10 * (F32::min(1.0, self.fRec17[0] + 1.0) * fTemp26 * fTemp23 * fTemp25 / fTemp24);
			self.fVec0[1] = self.fVec0[0];
			self.fRec2[1] = self.fRec2[0];
			self.fRec4[1] = self.fRec4[0];
			self.fRec6[1] = self.fRec6[0];
			self.fVec1[1] = self.fVec1[0];
			self.fRec9[1] = self.fRec9[0];
			self.fRec8[2] = self.fRec8[1];
			self.fRec8[1] = self.fRec8[0];
			self.fRec10[1] = self.fRec10[0];
			self.fRec12[1] = self.fRec12[0];
			self.fRec11[2] = self.fRec11[1];
			self.fRec11[1] = self.fRec11[0];
			self.fRec7[2] = self.fRec7[1];
			self.fRec7[1] = self.fRec7[0];
			self.fRec5[2] = self.fRec5[1];
			self.fRec5[1] = self.fRec5[0];
			self.fRec3[2] = self.fRec3[1];
			self.fRec3[1] = self.fRec3[0];
			self.fVec2[1] = self.fVec2[0];
			self.fRec1[1] = self.fRec1[0];
			self.fRec0[2] = self.fRec0[1];
			self.fRec0[1] = self.fRec0[0];
			self.fRec13[1] = self.fRec13[0];
			self.fRec15[1] = self.fRec15[0];
			self.fRec14[2] = self.fRec14[1];
			self.fRec14[1] = self.fRec14[0];
			self.fRec16[1] = self.fRec16[0];
			self.fRec17[1] = self.fRec17[0];
		}
	}

}

