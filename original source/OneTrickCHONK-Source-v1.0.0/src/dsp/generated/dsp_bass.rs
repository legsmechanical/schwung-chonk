/* ------------------------------------------------------------
author: "Punk Labs LLC"
copyright: "2024 Punk Labs LLC"
name: "OneTrick CHONK Bass"
version: "1.0"
Code generated with Faust 2.75.7 (https://faust.grame.fr)
Compilation options: -a arch.rs -lang rust -ct 0 -cn DSP_Bass -dtl 65536 -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0
------------------------------------------------------------ */
use faust_types::*;
use onetrick::faust::FaustDspExtended;




pub struct DSP_BassSIG0 {
	iVec14: [i32;2],
	iRec54: [i32;2],
}

impl DSP_BassSIG0 {
	
	fn get_num_inputsDSP_BassSIG0(&self) -> i32 {
		return 0;
	}
	fn get_num_outputsDSP_BassSIG0(&self) -> i32 {
		return 1;
	}
	
	fn instance_initDSP_BassSIG0(&mut self, sample_rate: i32) {
		for l46 in 0..2 {
			self.iVec14[l46 as usize] = 0;
		}
		for l47 in 0..2 {
			self.iRec54[l47 as usize] = 0;
		}
	}
	
	fn fillDSP_BassSIG0(&mut self, count: i32, table: &mut[F32]) {
		for i1 in 0..count {
			self.iVec14[0] = 1;
			self.iRec54[0] = (i32::wrapping_add(self.iRec54[1], self.iVec14[1])) % 65536;
			table[i1 as usize] = F32::sin(9.58738e-05 * (self.iRec54[0]) as F32);
			self.iVec14[1] = self.iVec14[0];
			self.iRec54[1] = self.iRec54[0];
		}
	}

}


pub fn newDSP_BassSIG0() -> DSP_BassSIG0 { 
	DSP_BassSIG0 {
		iVec14: [0;2],
		iRec54: [0;2],
	}
}
fn DSP_Bass_faustpower2_f(value: F32) -> F32 {
	return value * value;
}
static mut ftbl0DSP_BassSIG0: [F32;65536] = [0.0;65536];
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
pub struct DSP_Bass {
	iVec0: [i32;2],
	fSampleRate: i32,
	fConst0: F32,
	fConst1: F32,
	fConst2: F32,
	fHslider0: F32,
	fConst3: F32,
	fConst4: F32,
	fConst5: F32,
	fConst6: F32,
	fConst7: F32,
	fConst8: F32,
	fConst9: F32,
	fConst10: F32,
	fConst11: F32,
	fConst12: F32,
	fConst13: F32,
	fButton0: F32,
	fVec1: [F32;2],
	fConst14: F32,
	fHslider1: F32,
	fRec26: [F32;2],
	fConst15: F32,
	fEntry0: F32,
	fHslider2: F32,
	fEntry1: F32,
	fRec28: [F32;2],
	fConst16: F32,
	fButton1: F32,
	fRec30: [F32;2],
	fConst17: F32,
	fEntry2: F32,
	fRec29: [F32;2],
	fEntry3: F32,
	fHslider3: F32,
	fRec31: [F32;2],
	fEntry4: F32,
	fVec2: [F32;2],
	fVec3: [F32;2],
	fEntry5: F32,
	fEntry6: F32,
	fRec32: [F32;2],
	fRec27: [F32;2],
	fButton2: F32,
	fRec33: [F32;2],
	fHslider4: F32,
	fEntry7: F32,
	fButton3: F32,
	fRec34: [F32;2],
	fVec4: [F32;2],
	fConst18: F32,
	fRec24: [F32;2],
	fRec25: [F32;2],
	fVec5: [F32;2],
	fHslider5: F32,
	fConst19: F32,
	fConst20: F32,
	fConst21: F32,
	fConst22: F32,
	fRec23: [F32;2],
	fVec6: [F32;2],
	fConst23: F32,
	fConst24: F32,
	fConst25: F32,
	fConst26: F32,
	fRec22: [F32;2],
	iVec7: [i32;2],
	iVec8: [i32;2],
	fConst27: F32,
	fRec37: [F32;2],
	fConst28: F32,
	iVec9: [i32;2],
	iRec38: [i32;2],
	fConst29: F32,
	fConst30: F32,
	fHslider6: F32,
	fButton4: F32,
	fRec39: [F32;2],
	fEntry8: F32,
	fRec40: [F32;2],
	fButton5: F32,
	fRec41: [F32;2],
	fButton6: F32,
	fRec42: [F32;2],
	fConst31: F32,
	fVec10: [F32;2],
	fRec43: [F32;2],
	fConst32: F32,
	iConst33: i32,
	iRec47: [i32;2],
	iRec48: [i32;2],
	fRec46: [F32;2],
	fRec44: [F32;2],
	fRec45: [F32;2],
	fVec11: [F32;2],
	fConst34: F32,
	fConst35: F32,
	fConst36: F32,
	fRec36: [F32;2],
	fConst37: F32,
	fConst38: F32,
	fConst39: F32,
	fRec35: [F32;2],
	fRec50: [F32;2],
	iRec51: [i32;2],
	fConst40: F32,
	fVec12: [F32;2],
	fRec52: [F32;2],
	fVec13: [F32;3],
	fRec49: [F32;2],
	fRec53: [F32;2],
	fConst41: F32,
	fRec55: [F32;2],
	fConst42: F32,
	fRec56: [F32;2],
	fConst43: F32,
	fRec57: [F32;2],
	fConst44: F32,
	fRec58: [F32;2],
	fRec60: [F32;2],
	fVec15: [F32;2],
	fRec63: [F32;2],
	fVec16: [F32;2],
	fRec62: [F32;2],
	IOTA0: i32,
	fRec61: [F32;512],
	fConst45: F32,
	fHslider7: F32,
	fConst46: F32,
	fVec17: [F32;2],
	fConst47: F32,
	fRec59: [F32;2],
	fVec18: [F32;8192],
	fRec20: [F32;2],
	fHslider8: F32,
	fRec15: [F32;8192],
	fRec11: [F32;2],
	fRec7: [F32;512],
	fRec5: [F32;2],
	fRec6: [F32;2],
	fConst48: F32,
	fConst49: F32,
	fConst50: F32,
	fConst51: F32,
	fRec4: [F32;2],
	fConst52: F32,
	fConst53: F32,
	fConst54: F32,
	fConst55: F32,
	fConst56: F32,
	fRec3: [F32;2],
	fConst57: F32,
	fRec2: [F32;3],
	fConst58: F32,
	fConst59: F32,
	fConst60: F32,
	fVec19: [F32;2],
	fConst61: F32,
	fRec1: [F32;2],
	fConst62: F32,
	fRec0: [F32;2],
}

impl FaustDsp for DSP_Bass {
	type T = F32;
		
	fn new() -> DSP_Bass { 
		DSP_Bass {
			iVec0: [0;2],
			fSampleRate: 0,
			fConst0: 0.0,
			fConst1: 0.0,
			fConst2: 0.0,
			fHslider0: 0.0,
			fConst3: 0.0,
			fConst4: 0.0,
			fConst5: 0.0,
			fConst6: 0.0,
			fConst7: 0.0,
			fConst8: 0.0,
			fConst9: 0.0,
			fConst10: 0.0,
			fConst11: 0.0,
			fConst12: 0.0,
			fConst13: 0.0,
			fButton0: 0.0,
			fVec1: [0.0;2],
			fConst14: 0.0,
			fHslider1: 0.0,
			fRec26: [0.0;2],
			fConst15: 0.0,
			fEntry0: 0.0,
			fHslider2: 0.0,
			fEntry1: 0.0,
			fRec28: [0.0;2],
			fConst16: 0.0,
			fButton1: 0.0,
			fRec30: [0.0;2],
			fConst17: 0.0,
			fEntry2: 0.0,
			fRec29: [0.0;2],
			fEntry3: 0.0,
			fHslider3: 0.0,
			fRec31: [0.0;2],
			fEntry4: 0.0,
			fVec2: [0.0;2],
			fVec3: [0.0;2],
			fEntry5: 0.0,
			fEntry6: 0.0,
			fRec32: [0.0;2],
			fRec27: [0.0;2],
			fButton2: 0.0,
			fRec33: [0.0;2],
			fHslider4: 0.0,
			fEntry7: 0.0,
			fButton3: 0.0,
			fRec34: [0.0;2],
			fVec4: [0.0;2],
			fConst18: 0.0,
			fRec24: [0.0;2],
			fRec25: [0.0;2],
			fVec5: [0.0;2],
			fHslider5: 0.0,
			fConst19: 0.0,
			fConst20: 0.0,
			fConst21: 0.0,
			fConst22: 0.0,
			fRec23: [0.0;2],
			fVec6: [0.0;2],
			fConst23: 0.0,
			fConst24: 0.0,
			fConst25: 0.0,
			fConst26: 0.0,
			fRec22: [0.0;2],
			iVec7: [0;2],
			iVec8: [0;2],
			fConst27: 0.0,
			fRec37: [0.0;2],
			fConst28: 0.0,
			iVec9: [0;2],
			iRec38: [0;2],
			fConst29: 0.0,
			fConst30: 0.0,
			fHslider6: 0.0,
			fButton4: 0.0,
			fRec39: [0.0;2],
			fEntry8: 0.0,
			fRec40: [0.0;2],
			fButton5: 0.0,
			fRec41: [0.0;2],
			fButton6: 0.0,
			fRec42: [0.0;2],
			fConst31: 0.0,
			fVec10: [0.0;2],
			fRec43: [0.0;2],
			fConst32: 0.0,
			iConst33: 0,
			iRec47: [0;2],
			iRec48: [0;2],
			fRec46: [0.0;2],
			fRec44: [0.0;2],
			fRec45: [0.0;2],
			fVec11: [0.0;2],
			fConst34: 0.0,
			fConst35: 0.0,
			fConst36: 0.0,
			fRec36: [0.0;2],
			fConst37: 0.0,
			fConst38: 0.0,
			fConst39: 0.0,
			fRec35: [0.0;2],
			fRec50: [0.0;2],
			iRec51: [0;2],
			fConst40: 0.0,
			fVec12: [0.0;2],
			fRec52: [0.0;2],
			fVec13: [0.0;3],
			fRec49: [0.0;2],
			fRec53: [0.0;2],
			fConst41: 0.0,
			fRec55: [0.0;2],
			fConst42: 0.0,
			fRec56: [0.0;2],
			fConst43: 0.0,
			fRec57: [0.0;2],
			fConst44: 0.0,
			fRec58: [0.0;2],
			fRec60: [0.0;2],
			fVec15: [0.0;2],
			fRec63: [0.0;2],
			fVec16: [0.0;2],
			fRec62: [0.0;2],
			IOTA0: 0,
			fRec61: [0.0;512],
			fConst45: 0.0,
			fHslider7: 0.0,
			fConst46: 0.0,
			fVec17: [0.0;2],
			fConst47: 0.0,
			fRec59: [0.0;2],
			fVec18: [0.0;8192],
			fRec20: [0.0;2],
			fHslider8: 0.0,
			fRec15: [0.0;8192],
			fRec11: [0.0;2],
			fRec7: [0.0;512],
			fRec5: [0.0;2],
			fRec6: [0.0;2],
			fConst48: 0.0,
			fConst49: 0.0,
			fConst50: 0.0,
			fConst51: 0.0,
			fRec4: [0.0;2],
			fConst52: 0.0,
			fConst53: 0.0,
			fConst54: 0.0,
			fConst55: 0.0,
			fConst56: 0.0,
			fRec3: [0.0;2],
			fConst57: 0.0,
			fRec2: [0.0;3],
			fConst58: 0.0,
			fConst59: 0.0,
			fConst60: 0.0,
			fVec19: [0.0;2],
			fConst61: 0.0,
			fRec1: [0.0;2],
			fConst62: 0.0,
			fRec0: [0.0;2],
		}
	}
	fn metadata(&self, m: &mut dyn Meta) { 
		m.declare("analyzers.lib/name", r"Faust Analyzer Library");
		m.declare("analyzers.lib/version", r"1.2.0");
		m.declare("author", r"Punk Labs LLC");
		m.declare("basics.lib/downSample:author", r"Romain Michon");
		m.declare("basics.lib/name", r"Faust Basic Element Library");
		m.declare("basics.lib/sAndH:author", r"Romain Michon");
		m.declare("basics.lib/tabulateNd", r"Copyright (C) 2023 Bart Brouns <bart@magnetophon.nl>");
		m.declare("basics.lib/version", r"1.19.1");
		m.declare("compile_options", r"-a arch.rs -lang rust -ct 0 -cn DSP_Bass -dtl 65536 -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0");
		m.declare("copyright", r"2024 Punk Labs LLC");
		m.declare("delays.lib/fdelay4:author", r"Julius O. Smith III");
		m.declare("delays.lib/fdelayltv:author", r"Julius O. Smith III");
		m.declare("delays.lib/name", r"Faust Delay Library");
		m.declare("delays.lib/version", r"1.1.0");
		m.declare("envelopes.lib/ar:author", r"Yann Orlarey, Stéphane Letz");
		m.declare("envelopes.lib/author", r"GRAME");
		m.declare("envelopes.lib/copyright", r"GRAME");
		m.declare("envelopes.lib/license", r"LGPL with exception");
		m.declare("envelopes.lib/name", r"Faust Envelope Library");
		m.declare("envelopes.lib/version", r"1.3.0");
		m.declare("filename", r"bass.dsp");
		m.declare("filters.lib/dcblockerat:author", r"Julius O. Smith III");
		m.declare("filters.lib/dcblockerat:copyright", r"Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m.declare("filters.lib/dcblockerat:license", r"MIT-style STK-4.3 license");
		m.declare("filters.lib/fir:author", r"Julius O. Smith III");
		m.declare("filters.lib/fir:copyright", r"Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m.declare("filters.lib/fir:license", r"MIT-style STK-4.3 license");
		m.declare("filters.lib/highpass:author", r"Julius O. Smith III");
		m.declare("filters.lib/highpass:copyright", r"Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m.declare("filters.lib/iir:author", r"Julius O. Smith III");
		m.declare("filters.lib/iir:copyright", r"Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m.declare("filters.lib/iir:license", r"MIT-style STK-4.3 license");
		m.declare("filters.lib/lowpass0_highpass1", r"MIT-style STK-4.3 license");
		m.declare("filters.lib/lowpass0_highpass1:author", r"Julius O. Smith III");
		m.declare("filters.lib/lowpass:author", r"Julius O. Smith III");
		m.declare("filters.lib/lowpass:copyright", r"Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m.declare("filters.lib/lowpass:license", r"MIT-style STK-4.3 license");
		m.declare("filters.lib/name", r"Faust Filters Library");
		m.declare("filters.lib/peak_eq:author", r"Julius O. Smith III");
		m.declare("filters.lib/peak_eq:copyright", r"Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m.declare("filters.lib/peak_eq:license", r"MIT-style STK-4.3 license");
		m.declare("filters.lib/peak_eq_cq:author", r"Julius O. Smith III");
		m.declare("filters.lib/peak_eq_cq:copyright", r"Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m.declare("filters.lib/peak_eq_cq:license", r"MIT-style STK-4.3 license");
		m.declare("filters.lib/pole:author", r"Julius O. Smith III");
		m.declare("filters.lib/pole:copyright", r"Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m.declare("filters.lib/pole:license", r"MIT-style STK-4.3 license");
		m.declare("filters.lib/spectral_tilt:author", r"Julius O. Smith III");
		m.declare("filters.lib/spectral_tilt:copyright", r"Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m.declare("filters.lib/spectral_tilt:license", r"MIT-style STK-4.3 license");
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
		m.declare("filters.lib/zero:author", r"Julius O. Smith III");
		m.declare("filters.lib/zero:copyright", r"Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m.declare("filters.lib/zero:license", r"MIT-style STK-4.3 license");
		m.declare("interpolators.lib/interpolate_linear:author", r"Stéphane Letz");
		m.declare("interpolators.lib/interpolate_linear:licence", r"MIT");
		m.declare("interpolators.lib/name", r"Faust Interpolator Library");
		m.declare("interpolators.lib/remap:author", r"David Braun");
		m.declare("interpolators.lib/version", r"1.3.1");
		m.declare("maths.lib/author", r"GRAME");
		m.declare("maths.lib/copyright", r"GRAME");
		m.declare("maths.lib/license", r"LGPL with exception");
		m.declare("maths.lib/name", r"Faust Math Library");
		m.declare("maths.lib/version", r"2.8.0");
		m.declare("name", r"OneTrick CHONK Bass");
		m.declare("noises.lib/name", r"Faust Noise Generator Library");
		m.declare("noises.lib/version", r"1.4.1");
		m.declare("onetrick.lib/copyright", r"Copyright (c) 2023 Punk Labs LLC");
		m.declare("onetrick.lib/license", r"GPLv3 (or later)");
		m.declare("onetrick.lib/name", r"OneTrick DSP Library");
		m.declare("oscillators.lib/hs_oscsin:author", r"Mike Olsen");
		m.declare("oscillators.lib/hs_phasor:author", r"Mike Olsen, revised by Stéphane Letz");
		m.declare("oscillators.lib/name", r"Faust Oscillator Library");
		m.declare("oscillators.lib/version", r"1.5.1");
		m.declare("physmodels.lib/name", r"Faust Physical Models Library");
		m.declare("physmodels.lib/version", r"1.1.0");
		m.declare("platform.lib/name", r"Generic Platform Library");
		m.declare("platform.lib/version", r"1.3.0");
		m.declare("routes.lib/name", r"Faust Signal Routing Library");
		m.declare("routes.lib/version", r"1.2.0");
		m.declare("signals.lib/name", r"Faust Signal Routing Library");
		m.declare("signals.lib/version", r"1.6.0");
		m.declare("version", r"1.0");
	}

	fn get_sample_rate(&self) -> i32 {
		return self.fSampleRate;
	}
	fn get_num_inputs(&self) -> i32 {
		return 0;
	}
	fn get_num_outputs(&self) -> i32 {
		return 1;
	}
	
	fn class_init(sample_rate: i32) {
		let mut sig0: DSP_BassSIG0 = newDSP_BassSIG0();
		sig0.instance_initDSP_BassSIG0(sample_rate);
		sig0.fillDSP_BassSIG0(65536, unsafe { &mut ftbl0DSP_BassSIG0 });
	}
	fn instance_reset_params(&mut self) {
		self.fHslider0 = 5e+01;
		self.fButton0 = 0.0;
		self.fHslider1 = 0.0;
		self.fEntry0 = 0.0;
		self.fHslider2 = 2.0;
		self.fEntry1 = 0.0;
		self.fButton1 = 0.0;
		self.fEntry2 = 36.0;
		self.fEntry3 = 0.0;
		self.fHslider3 = 1.0;
		self.fEntry4 = 0.0;
		self.fEntry5 = 0.0;
		self.fEntry6 = 0.0;
		self.fButton2 = 0.0;
		self.fHslider4 = 1e+02;
		self.fEntry7 = 0.0;
		self.fButton3 = 0.0;
		self.fHslider5 = 5e+01;
		self.fHslider6 = 0.0;
		self.fButton4 = 0.0;
		self.fEntry8 = 0.0;
		self.fButton5 = 0.0;
		self.fButton6 = 0.0;
		self.fHslider7 = 5e+01;
		self.fHslider8 = 5e+01;
	}
	fn instance_clear(&mut self) {
		for l0 in 0..2 {
			self.iVec0[l0 as usize] = 0;
		}
		for l1 in 0..2 {
			self.fVec1[l1 as usize] = 0.0;
		}
		for l2 in 0..2 {
			self.fRec26[l2 as usize] = 0.0;
		}
		for l3 in 0..2 {
			self.fRec28[l3 as usize] = 0.0;
		}
		for l4 in 0..2 {
			self.fRec30[l4 as usize] = 0.0;
		}
		for l5 in 0..2 {
			self.fRec29[l5 as usize] = 0.0;
		}
		for l6 in 0..2 {
			self.fRec31[l6 as usize] = 0.0;
		}
		for l7 in 0..2 {
			self.fVec2[l7 as usize] = 0.0;
		}
		for l8 in 0..2 {
			self.fVec3[l8 as usize] = 0.0;
		}
		for l9 in 0..2 {
			self.fRec32[l9 as usize] = 0.0;
		}
		for l10 in 0..2 {
			self.fRec27[l10 as usize] = 0.0;
		}
		for l11 in 0..2 {
			self.fRec33[l11 as usize] = 0.0;
		}
		for l12 in 0..2 {
			self.fRec34[l12 as usize] = 0.0;
		}
		for l13 in 0..2 {
			self.fVec4[l13 as usize] = 0.0;
		}
		for l14 in 0..2 {
			self.fRec24[l14 as usize] = 0.0;
		}
		for l15 in 0..2 {
			self.fRec25[l15 as usize] = 0.0;
		}
		for l16 in 0..2 {
			self.fVec5[l16 as usize] = 0.0;
		}
		for l17 in 0..2 {
			self.fRec23[l17 as usize] = 0.0;
		}
		for l18 in 0..2 {
			self.fVec6[l18 as usize] = 0.0;
		}
		for l19 in 0..2 {
			self.fRec22[l19 as usize] = 0.0;
		}
		for l20 in 0..2 {
			self.iVec7[l20 as usize] = 0;
		}
		for l21 in 0..2 {
			self.iVec8[l21 as usize] = 0;
		}
		for l22 in 0..2 {
			self.fRec37[l22 as usize] = 0.0;
		}
		for l23 in 0..2 {
			self.iVec9[l23 as usize] = 0;
		}
		for l24 in 0..2 {
			self.iRec38[l24 as usize] = 0;
		}
		for l25 in 0..2 {
			self.fRec39[l25 as usize] = 0.0;
		}
		for l26 in 0..2 {
			self.fRec40[l26 as usize] = 0.0;
		}
		for l27 in 0..2 {
			self.fRec41[l27 as usize] = 0.0;
		}
		for l28 in 0..2 {
			self.fRec42[l28 as usize] = 0.0;
		}
		for l29 in 0..2 {
			self.fVec10[l29 as usize] = 0.0;
		}
		for l30 in 0..2 {
			self.fRec43[l30 as usize] = 0.0;
		}
		for l31 in 0..2 {
			self.iRec47[l31 as usize] = 0;
		}
		for l32 in 0..2 {
			self.iRec48[l32 as usize] = 0;
		}
		for l33 in 0..2 {
			self.fRec46[l33 as usize] = 0.0;
		}
		for l34 in 0..2 {
			self.fRec44[l34 as usize] = 0.0;
		}
		for l35 in 0..2 {
			self.fRec45[l35 as usize] = 0.0;
		}
		for l36 in 0..2 {
			self.fVec11[l36 as usize] = 0.0;
		}
		for l37 in 0..2 {
			self.fRec36[l37 as usize] = 0.0;
		}
		for l38 in 0..2 {
			self.fRec35[l38 as usize] = 0.0;
		}
		for l39 in 0..2 {
			self.fRec50[l39 as usize] = 0.0;
		}
		for l40 in 0..2 {
			self.iRec51[l40 as usize] = 0;
		}
		for l41 in 0..2 {
			self.fVec12[l41 as usize] = 0.0;
		}
		for l42 in 0..2 {
			self.fRec52[l42 as usize] = 0.0;
		}
		for l43 in 0..3 {
			self.fVec13[l43 as usize] = 0.0;
		}
		for l44 in 0..2 {
			self.fRec49[l44 as usize] = 0.0;
		}
		for l45 in 0..2 {
			self.fRec53[l45 as usize] = 0.0;
		}
		for l48 in 0..2 {
			self.fRec55[l48 as usize] = 0.0;
		}
		for l49 in 0..2 {
			self.fRec56[l49 as usize] = 0.0;
		}
		for l50 in 0..2 {
			self.fRec57[l50 as usize] = 0.0;
		}
		for l51 in 0..2 {
			self.fRec58[l51 as usize] = 0.0;
		}
		for l52 in 0..2 {
			self.fRec60[l52 as usize] = 0.0;
		}
		for l53 in 0..2 {
			self.fVec15[l53 as usize] = 0.0;
		}
		for l54 in 0..2 {
			self.fRec63[l54 as usize] = 0.0;
		}
		for l55 in 0..2 {
			self.fVec16[l55 as usize] = 0.0;
		}
		for l56 in 0..2 {
			self.fRec62[l56 as usize] = 0.0;
		}
		self.IOTA0 = 0;
		for l57 in 0..512 {
			self.fRec61[l57 as usize] = 0.0;
		}
		for l58 in 0..2 {
			self.fVec17[l58 as usize] = 0.0;
		}
		for l59 in 0..2 {
			self.fRec59[l59 as usize] = 0.0;
		}
		for l60 in 0..8192 {
			self.fVec18[l60 as usize] = 0.0;
		}
		for l61 in 0..2 {
			self.fRec20[l61 as usize] = 0.0;
		}
		for l62 in 0..8192 {
			self.fRec15[l62 as usize] = 0.0;
		}
		for l63 in 0..2 {
			self.fRec11[l63 as usize] = 0.0;
		}
		for l64 in 0..512 {
			self.fRec7[l64 as usize] = 0.0;
		}
		for l65 in 0..2 {
			self.fRec5[l65 as usize] = 0.0;
		}
		for l66 in 0..2 {
			self.fRec6[l66 as usize] = 0.0;
		}
		for l67 in 0..2 {
			self.fRec4[l67 as usize] = 0.0;
		}
		for l68 in 0..2 {
			self.fRec3[l68 as usize] = 0.0;
		}
		for l69 in 0..3 {
			self.fRec2[l69 as usize] = 0.0;
		}
		for l70 in 0..2 {
			self.fVec19[l70 as usize] = 0.0;
		}
		for l71 in 0..2 {
			self.fRec1[l71 as usize] = 0.0;
		}
		for l72 in 0..2 {
			self.fRec0[l72 as usize] = 0.0;
		}
	}
	fn instance_constants(&mut self, sample_rate: i32) {
		self.fSampleRate = sample_rate;
		self.fConst0 = F32::min(1.92e+05, F32::max(1.0, (self.fSampleRate) as F32));
		self.fConst1 = 62.831852 / self.fConst0;
		self.fConst2 = 1.0 - self.fConst1;
		self.fConst3 = 1382.3008 / self.fConst0;
		self.fConst4 = F32::tan(97.38937 / self.fConst0);
		self.fConst5 = 2.0 * (1.0 - 1.0 / DSP_Bass_faustpower2_f(self.fConst4));
		self.fConst6 = self.fConst0 * F32::sin(194.77875 / self.fConst0);
		self.fConst7 = 194.77875 / self.fConst6;
		self.fConst8 = 1.0 / self.fConst4;
		self.fConst9 = (self.fConst8 - self.fConst7) / self.fConst4 + 1.0;
		self.fConst10 = (self.fConst8 + self.fConst7) / self.fConst4 + 1.0;
		self.fConst11 = 1.0 / self.fConst10;
		self.fConst12 = 1.0 / F32::tan(0.5 / self.fConst0);
		self.fConst13 = 12566.371 - self.fConst12;
		self.fConst14 = F32::exp(-(1e+02 / self.fConst0));
		self.fConst15 = F32::exp(-(5e+02 / self.fConst0));
		self.fConst16 = F32::exp(-(1e+03 / self.fConst0));
		self.fConst17 = 1.0 / self.fConst0;
		self.fConst18 = 0.002 * self.fConst0;
		self.fConst19 = 6283.1855 / self.fConst0;
		self.fConst20 = F32::tan(self.fConst19);
		self.fConst21 = 12566.371 / self.fConst20;
		self.fConst22 = 1.0 / (self.fConst12 + 12566.371);
		self.fConst23 = F32::tan(20420.352 / self.fConst0);
		self.fConst24 = 12566.371 * (self.fConst23 / self.fConst20);
		self.fConst25 = self.fConst24 - self.fConst12;
		self.fConst26 = 1.0 / (self.fConst12 + self.fConst24);
		self.fConst27 = 0.0002 * self.fConst0;
		self.fConst28 = 5e+03 / self.fConst0;
		self.fConst29 = F32::max(1.0, self.fConst27);
		self.fConst30 = 1.0 / self.fConst29;
		self.fConst31 = 0.001 * self.fConst0;
		self.fConst32 = self.fConst0 / F32::min(F32::min(4.8e+04, self.fConst0), self.fConst0);
		self.iConst33 = (self.fConst32) as i32;
		self.fConst34 = 1.0 / F32::tan(942.4778 / self.fConst0);
		self.fConst35 = 1.0 - self.fConst34;
		self.fConst36 = 1.0 / (self.fConst34 + 1.0);
		self.fConst37 = 1.0 / F32::tan(11780.973 / self.fConst0);
		self.fConst38 = 1.0 - self.fConst37;
		self.fConst39 = 1.0 / (self.fConst37 + 1.0);
		self.fConst40 = 0.0045454544 * self.fConst0;
		self.fConst41 = 1.76e+03 / self.fConst0;
		self.fConst42 = 1.32e+03 / self.fConst0;
		self.fConst43 = 8.8e+02 / self.fConst0;
		self.fConst44 = 4.4e+02 / self.fConst0;
		self.fConst45 = 0.03057805 * self.fConst0;
		self.fConst46 = 2.9411765e-05 * self.fConst0;
		self.fConst47 = 1.0 - self.fConst14;
		self.fConst48 = F32::tan(14457.107 / self.fConst0);
		self.fConst49 = 12566.371 * (self.fConst48 / self.fConst20);
		self.fConst50 = self.fConst49 - self.fConst12;
		self.fConst51 = self.fConst12 + self.fConst49;
		self.fConst52 = F32::tan(46985.598 / self.fConst0);
		self.fConst53 = 12566.371 * (self.fConst52 / self.fConst20);
		self.fConst54 = self.fConst53 - self.fConst12;
		self.fConst55 = self.fConst12 + self.fConst53;
		self.fConst56 = self.fConst20 / self.fConst48;
		self.fConst57 = self.fConst23 / self.fConst52;
		self.fConst58 = 388.63467 / self.fConst6;
		self.fConst59 = (self.fConst8 - self.fConst58) / self.fConst4 + 1.0;
		self.fConst60 = (self.fConst8 + self.fConst58) / self.fConst4 + 1.0;
		self.fConst61 = 0.70794576 / self.fConst10;
		self.fConst62 = 1.0 / (self.fConst1 + 1.0);
	}
	fn instance_init(&mut self, sample_rate: i32) {
		self.instance_constants(sample_rate);
		self.instance_reset_params();
		self.instance_clear();
	}
	fn init(&mut self, sample_rate: i32) {
		DSP_Bass::class_init(sample_rate);
		self.instance_init(sample_rate);
	}
	
	fn build_user_interface(&self, ui_interface: &mut dyn UI<Self::T>) {
		Self::build_user_interface_static(ui_interface);
	}
	
	fn build_user_interface_static(ui_interface: &mut dyn UI<Self::T>) {
		ui_interface.open_vertical_box("OneTrick CHONK Bass");
		ui_interface.add_num_entry("Aftertouch", ParamIndex(0), 0.0, 0.0, 1.0, 0.001);
		ui_interface.add_button("Articulation_Finger", ParamIndex(1));
		ui_interface.add_button("Articulation_Legato", ParamIndex(2));
		ui_interface.add_button("Articulation_Mute", ParamIndex(3));
		ui_interface.add_button("Articulation_Pick", ParamIndex(4));
		ui_interface.add_button("Articulation_Slap", ParamIndex(5));
		ui_interface.add_num_entry("Articulation_SlideDown", ParamIndex(6), 0.0, 0.0, 1.0, 0.01);
		ui_interface.add_num_entry("Articulation_SlideUp", ParamIndex(7), 0.0, 0.0, 1.0, 0.01);
		ui_interface.add_num_entry("ModWheel", ParamIndex(8), 0.0, 0.0, 1.0, 0.001);
		ui_interface.add_num_entry("PitchWheel", ParamIndex(9), 0.0, -1.0, 1.0, 0.001);
		ui_interface.add_num_entry("Transpose", ParamIndex(10), 0.0, -48.0, 48.0, 0.001);
		ui_interface.add_num_entry("Trigger", ParamIndex(11), 0.0, 0.0, 1.0, 0.01);
		ui_interface.add_button("WakeUp", ParamIndex(12));
		ui_interface.declare(Some(ParamIndex(13)), "210", "");
		ui_interface.declare(Some(ParamIndex(13)), "export", "Pickup Position");
		ui_interface.declare(Some(ParamIndex(13)), "group", "Bass");
		ui_interface.declare(Some(ParamIndex(13)), "unit", "%");
		ui_interface.add_horizontal_slider("Pickup_Position", ParamIndex(13), 5e+01, 0.0, 1e+02, 0.01);
		ui_interface.declare(Some(ParamIndex(14)), "220", "");
		ui_interface.declare(Some(ParamIndex(14)), "export", "Sustain");
		ui_interface.declare(Some(ParamIndex(14)), "group", "Bass");
		ui_interface.declare(Some(ParamIndex(14)), "unit", "%");
		ui_interface.add_horizontal_slider("Sustain", ParamIndex(14), 1e+02, 0.0, 1e+02, 0.01);
		ui_interface.declare(Some(ParamIndex(15)), "230", "");
		ui_interface.declare(Some(ParamIndex(15)), "export", "String Brightness");
		ui_interface.declare(Some(ParamIndex(15)), "group", "Bass");
		ui_interface.declare(Some(ParamIndex(15)), "unit", "%");
		ui_interface.add_horizontal_slider("Brightness", ParamIndex(15), 5e+01, 0.0, 1e+02, 0.01);
		ui_interface.declare(Some(ParamIndex(16)), "240", "");
		ui_interface.declare(Some(ParamIndex(16)), "export", "Strike Hardness");
		ui_interface.declare(Some(ParamIndex(16)), "group", "Bass");
		ui_interface.declare(Some(ParamIndex(16)), "unit", "%");
		ui_interface.add_horizontal_slider("StrikeHardness", ParamIndex(16), 0.0, 0.0, 1e+02, 0.01);
		ui_interface.declare(Some(ParamIndex(17)), "250", "");
		ui_interface.declare(Some(ParamIndex(17)), "export", "Thump");
		ui_interface.declare(Some(ParamIndex(17)), "group", "Bass");
		ui_interface.declare(Some(ParamIndex(17)), "unit", "%");
		ui_interface.add_horizontal_slider("Bass_Thump", ParamIndex(17), 5e+01, 0.0, 1e+02, 0.01);
		ui_interface.declare(Some(ParamIndex(18)), "260", "");
		ui_interface.declare(Some(ParamIndex(18)), "export", "Tone Knob");
		ui_interface.declare(Some(ParamIndex(18)), "group", "Bass");
		ui_interface.declare(Some(ParamIndex(18)), "unit", "%");
		ui_interface.add_horizontal_slider("Tone_Knob", ParamIndex(18), 5e+01, 0.0, 1e+02, 0.01);
		ui_interface.declare(Some(ParamIndex(19)), "310", "");
		ui_interface.declare(Some(ParamIndex(19)), "export", "Fine Tune");
		ui_interface.declare(Some(ParamIndex(19)), "group", "MIDI");
		ui_interface.declare(Some(ParamIndex(19)), "unit", "c");
		ui_interface.add_horizontal_slider("Fine_Tune", ParamIndex(19), 0.0, -1e+02, 1e+02, 0.01);
		ui_interface.declare(Some(ParamIndex(20)), "320", "");
		ui_interface.declare(Some(ParamIndex(20)), "export", "Aftertouch Range");
		ui_interface.declare(Some(ParamIndex(20)), "group", "MIDI");
		ui_interface.declare(Some(ParamIndex(20)), "unit", "st");
		ui_interface.add_horizontal_slider("Midi_Afterouch_range", ParamIndex(20), 1.0, -2.0, 2.0, 0.01);
		ui_interface.declare(Some(ParamIndex(21)), "330", "");
		ui_interface.declare(Some(ParamIndex(21)), "export", "PitchWheel Range");
		ui_interface.declare(Some(ParamIndex(21)), "group", "MIDI");
		ui_interface.declare(Some(ParamIndex(21)), "unit", "st");
		ui_interface.add_horizontal_slider("Midi_PitchWheel_range", ParamIndex(21), 2.0, 0.0, 12.0, 0.01);
		ui_interface.add_num_entry("gain", ParamIndex(22), 0.0, 0.0, 1.0, 0.01);
		ui_interface.add_button("gate", ParamIndex(23));
		ui_interface.add_num_entry("key", ParamIndex(24), 36.0, 0.0, 128.0, 1.0);
		ui_interface.close_box();
	}
	
	fn get_param(&self, param: ParamIndex) -> Option<Self::T> {
		match param.0 {
			12 => Some(self.fButton0),
			2 => Some(self.fButton1),
			3 => Some(self.fButton2),
			23 => Some(self.fButton3),
			4 => Some(self.fButton4),
			5 => Some(self.fButton5),
			1 => Some(self.fButton6),
			9 => Some(self.fEntry0),
			10 => Some(self.fEntry1),
			24 => Some(self.fEntry2),
			0 => Some(self.fEntry3),
			11 => Some(self.fEntry4),
			6 => Some(self.fEntry5),
			7 => Some(self.fEntry6),
			8 => Some(self.fEntry7),
			22 => Some(self.fEntry8),
			18 => Some(self.fHslider0),
			19 => Some(self.fHslider1),
			21 => Some(self.fHslider2),
			20 => Some(self.fHslider3),
			14 => Some(self.fHslider4),
			15 => Some(self.fHslider5),
			16 => Some(self.fHslider6),
			13 => Some(self.fHslider7),
			17 => Some(self.fHslider8),
			_ => None,
		}
	}
	
	fn set_param(&mut self, param: ParamIndex, value: Self::T) {
		match param.0 {
			12 => { self.fButton0 = value }
			2 => { self.fButton1 = value }
			3 => { self.fButton2 = value }
			23 => { self.fButton3 = value }
			4 => { self.fButton4 = value }
			5 => { self.fButton5 = value }
			1 => { self.fButton6 = value }
			9 => { self.fEntry0 = value }
			10 => { self.fEntry1 = value }
			24 => { self.fEntry2 = value }
			0 => { self.fEntry3 = value }
			11 => { self.fEntry4 = value }
			6 => { self.fEntry5 = value }
			7 => { self.fEntry6 = value }
			8 => { self.fEntry7 = value }
			22 => { self.fEntry8 = value }
			18 => { self.fHslider0 = value }
			19 => { self.fHslider1 = value }
			21 => { self.fHslider2 = value }
			20 => { self.fHslider3 = value }
			14 => { self.fHslider4 = value }
			15 => { self.fHslider5 = value }
			16 => { self.fHslider6 = value }
			13 => { self.fHslider7 = value }
			17 => { self.fHslider8 = value }
			_ => {}
		}
	}
	
	fn compute(&mut self, count: i32, inputs: &[&[Self::T]], outputs: &mut[&mut[Self::T]]) {
		let (outputs0) = if let [outputs0, ..] = outputs {
			let outputs0 = outputs0[..count as usize].iter_mut();
			(outputs0)
		} else {
			panic!("wrong number of outputs");
		};
		let mut fSlow0: F32 = 1.0 / F32::tan(self.fConst3 * F32::powf(2.0, 0.083333336 * (0.36706725 * self.fHslider0 + -6.630492)));
		let mut fSlow1: F32 = 1.0 - fSlow0;
		let mut fSlow2: F32 = self.fButton0;
		let mut fSlow3: F32 = 0.01 * self.fHslider1;
		let mut fSlow4: F32 = self.fEntry1 + self.fHslider2 * self.fEntry0;
		let mut fSlow5: F32 = self.fButton1;
		let mut fSlow6: F32 = F32::max(0.0, self.fEntry2 + -24.0);
		let mut fSlow7: F32 = self.fHslider3 * self.fEntry3;
		let mut fSlow8: F32 = self.fEntry4;
		let mut fSlow9: F32 = self.fEntry6 - self.fEntry5;
		let mut fSlow10: F32 = 48.0 * (((((48.0 * fSlow9) != 0.0) as i32) > 0) as i32) as u32 as F32 * fSlow9;
		let mut fSlow11: F32 = self.fButton2;
		let mut fSlow12: F32 = 0.01 * self.fHslider4;
		let mut fSlow13: F32 = 0.02 * ((-1.0 - fSlow12) * self.fEntry7 + fSlow12) + 0.98;
		let mut fSlow14: F32 = self.fButton3;
		let mut fSlow15: F32 = self.fHslider5;
		let mut fSlow16: F32 = F32::min(0.0, F32::max(-1.0, -(0.1 - 0.0006 * fSlow15)));
		let mut fSlow17: F32 = F32::tan(self.fConst19 * F32::powf(3.25, -fSlow16));
		let mut fSlow18: F32 = self.fConst21 * fSlow17;
		let mut fSlow19: F32 = fSlow18 - self.fConst12;
		let mut fSlow20: F32 = self.fConst12 + fSlow18;
		let mut fSlow21: F32 = self.fConst20 / fSlow17;
		let mut fSlow22: F32 = F32::tan(self.fConst19 * F32::powf(3.25, 1.0 - fSlow16));
		let mut fSlow23: F32 = self.fConst21 * fSlow22;
		let mut fSlow24: F32 = fSlow23 - self.fConst12;
		let mut fSlow25: F32 = self.fConst20 * ((self.fConst12 + fSlow23) / fSlow17);
		let mut fSlow26: F32 = self.fConst23 / fSlow22;
		let mut iSlow27: i32 = ((fSlow6 >= 24.0) as i32) * ((fSlow6 <= 67.0) as i32);
		let mut fSlow28: F32 = 0.01 * self.fHslider6;
		let mut fSlow29: F32 = self.fButton4;
		let mut fSlow30: F32 = self.fEntry8;
		let mut fSlow31: F32 = self.fButton5;
		let mut fSlow32: F32 = self.fButton6;
		let mut fSlow33: F32 = 0.08 * self.fHslider7;
		let mut fSlow34: F32 = 3.0 * (fSlow33 + 12.0);
		let mut fSlow35: F32 = 0.023255814 * (fSlow33 + (12.0 - fSlow34));
		let mut fSlow36: F32 = 0.003 * fSlow15;
		let mut fSlow37: F32 = 0.03 * self.fHslider8;
		let mut fSlow38: F32 = 1.5 - fSlow37;
		let mut fSlow39: F32 = 1.0 / (fSlow0 + 1.0);
		let zipped_iterators = outputs0;
		for output0 in zipped_iterators {
			self.iVec0[0] = 1;
			let mut fTemp0: F32 = self.fConst5 * self.fRec2[1];
			self.fVec1[0] = fSlow2;
			let mut fTemp1: F32 = ((fSlow2 <= self.fVec1[1]) as i32) as u32 as F32;
			let mut fTemp2: F32 = 1.0 - self.fConst14 * fTemp1;
			self.fRec26[0] = fSlow3 * fTemp2 + self.fConst14 * fTemp1 * self.fRec26[1];
			self.fRec28[0] = fSlow4 * fTemp2 + self.fConst14 * fTemp1 * self.fRec28[1];
			let mut fTemp3: F32 = 1.0 - self.fConst16 * fTemp1;
			self.fRec30[0] = fSlow5 * fTemp3 + self.fConst16 * fTemp1 * self.fRec30[1];
			let mut fTemp4: F32 = 0.035 * self.fRec30[0];
			let mut iTemp5: i32 = (F32::abs(fTemp4) < 1.1920929e-07) as i32;
			let mut fTemp6: F32 = if iTemp5 != 0 {0.0} else {F32::exp(-(self.fConst17 / if iTemp5 != 0 {1.0} else {fTemp4}))};
			self.fRec29[0] = fSlow6 * (1.0 - fTemp6) + fTemp6 * self.fRec29[1];
			self.fRec31[0] = fSlow7 * fTemp3 + self.fConst16 * fTemp1 * self.fRec31[1];
			self.fVec2[0] = fSlow8;
			let mut fTemp7: F32 = if (fSlow8 > self.fVec2[1]) as i32 != 0 {fSlow8} else {0.0};
			self.fVec3[0] = fTemp7;
			let mut iTemp8: i32 = (fTemp7 > self.fVec3[1]) as i32;
			self.fRec32[0] = fSlow10 + self.fRec32[1] * ((iTemp8 == 0) as i32) as u32 as F32;
			let mut fTemp9: F32 = self.fConst17 * self.fRec32[0];
			let mut fTemp10: F32 = self.fRec29[0] + fTemp9;
			let mut fTemp11: F32 = F32::floor(fTemp10);
			let mut fTemp12: F32 = fTemp10 - fTemp11;
			self.fRec27[0] = (1.0 - self.fConst15 * fTemp1) * F32::min(67.0, F32::max(24.0, (self.fRec30[0] + ((F32::abs(fTemp9) > 0.1) as i32) as u32 as F32) * (fTemp11 + if (fTemp12 < 0.4125) as i32 != 0 {0.0} else {if (fTemp12 < 0.5875) as i32 != 0 {5.714286 * (fTemp10 + (-0.4125 - fTemp11))} else {1.0}} - fTemp10) + fTemp9 + self.fRec31[0] + self.fRec29[0] + self.fRec28[0])) + self.fConst15 * fTemp1 * self.fRec27[1];
			let mut fTemp13: F32 = F32::powf(2.0, 0.083333336 * (self.fRec27[0] + -69.0)) * F32::powf(2.0, 0.083333336 * self.fRec26[0]);
			let mut fTemp14: F32 = 4.4e+02 * fTemp13 + -32.703197;
			self.fRec33[0] = fSlow11 * fTemp3 + self.fConst16 * fTemp1 * self.fRec33[1];
			self.fRec34[0] = fSlow14 * fTemp3 + self.fConst16 * fTemp1 * self.fRec34[1];
			let mut fTemp15: F32 = self.fRec34[0] * (fSlow13 * (1.5029548e-05 * fTemp14 + 0.9945) * (self.fRec33[0] * (0.00055664993 * fTemp14 + -0.25) + 1.0) + -0.5);
			let mut fTemp16: F32 = fTemp15 + 0.5;
			self.fVec4[0] = fTemp16;
			let mut fTemp17: F32 = if (fTemp16 != self.fVec4[1]) as i32 != 0 {self.fConst18} else {self.fRec24[1] + -1.0};
			self.fRec24[0] = fTemp17;
			self.fRec25[0] = if (fTemp17 > 0.0) as i32 != 0 {self.fRec25[1] + (fTemp15 + (0.5 - self.fRec25[1])) / fTemp17} else {fTemp16};
			let mut fTemp18: F32 = self.fRec25[0] * self.fRec20[1];
			self.fVec5[0] = fTemp18;
			self.fRec23[0] = -(self.fConst22 * (fSlow20 * fTemp18 + fSlow19 * self.fVec5[1] + self.fConst13 * self.fRec23[1]));
			self.fVec6[0] = fSlow21 * self.fRec23[0];
			self.fRec22[0] = -(self.fConst26 * (self.fConst25 * self.fRec22[1] - (fSlow25 * self.fRec23[0] + fSlow24 * self.fVec6[1])));
			let mut fRec19: F32 = fSlow26 * self.fRec22[0];
			let mut fTemp19: F32 = 1.0 - self.fRec30[0];
			let mut iTemp20: i32 = if iSlow27 != 0 {iTemp8} else {0};
			self.iVec7[0] = iTemp20;
			let mut iTemp21: i32 = (iTemp20 > self.iVec7[1]) as i32;
			self.iVec8[0] = iTemp21;
			self.fRec37[0] = if (iTemp21 > 0) as i32 != 0 {self.fConst27} else {F32::max(0.0, self.fRec37[1] + -1.0)};
			let mut iTemp22: i32 = (iTemp21 > self.iVec8[1]) as i32;
			self.iVec9[0] = iTemp22;
			self.iRec38[0] = i32::wrapping_add(iTemp22, i32::wrapping_mul(i32::wrapping_add(self.iRec38[1], (self.iRec38[1] > 0) as i32), (iTemp21 <= self.iVec8[1]) as i32));
			let mut fTemp23: F32 = (self.iRec38[0]) as F32;
			self.fRec39[0] = fSlow29 * fTemp3 + self.fConst16 * fTemp1 * self.fRec39[1];
			self.fRec40[0] = if iTemp8 != 0 {fTemp7} else {self.fRec40[1]};
			let mut fTemp24: F32 = F32::max(fSlow30, self.fRec40[0]);
			self.fRec41[0] = fSlow31 * fTemp3 + self.fConst16 * fTemp1 * self.fRec41[1];
			let mut fTemp25: F32 = F32::max(F32::powf(F32::max(0.0, 6.6666665 * (fTemp24 + -0.85)), 1.5), self.fRec41[0]);
			self.fRec42[0] = fSlow32 * fTemp3 + self.fConst16 * fTemp1 * self.fRec42[1];
			let mut fTemp26: F32 = fSlow28 + F32::max(fTemp25, F32::max(self.fRec39[0], self.fRec42[0])) * (F32::max(0.5 * self.fRec39[0], fTemp25) - fSlow28);
			let mut fTemp27: F32 = F32::powf(fTemp24, 1.5);
			let mut fTemp28: F32 = F32::max(0.0, F32::min(self.fConst30 * fTemp23, (self.fConst29 - fTemp23) / F32::max(1.0, self.fConst31 * (6.0 * fTemp27 * fTemp26 + 2.0)) + 1.0));
			self.fVec10[0] = fTemp28;
			self.fRec43[0] = if iTemp21 != 0 {self.fVec10[1]} else {self.fRec43[1]};
			self.iRec47[0] = i32::wrapping_add(self.iRec47[1], 1);
			self.iRec48[0] = i32::wrapping_add(i32::wrapping_mul(1103515245, self.iRec48[1]), 12345);
			self.fRec46[0] = if (((i32::wrapping_add(self.iRec47[0], -1)) % self.iConst33) == 0) as i32 != 0 {4.656613e-10 * (self.iRec48[0]) as F32} else {self.fRec46[1]};
			let mut fTemp29: F32 = if (self.fRec46[0] != self.fRec46[1]) as i32 != 0 {self.fConst32} else {self.fRec44[1] + -1.0};
			self.fRec44[0] = fTemp29;
			self.fRec45[0] = if (fTemp29 > 0.0) as i32 != 0 {self.fRec45[1] + (self.fRec46[0] - self.fRec45[1]) / fTemp29} else {self.fRec46[0]};
			let mut fTemp30: F32 = self.fRec45[0] * fTemp27 * fTemp26 * if ((self.fConst28 * self.fRec37[0]) > 0.0) as i32 != 0 {fTemp28 + self.fConst28 * self.fRec37[0] * (self.fRec43[0] - fTemp28)} else {fTemp28};
			self.fVec11[0] = fTemp30;
			self.fRec36[0] = -(self.fConst36 * (self.fConst35 * self.fRec36[1] - self.fConst34 * (fTemp30 - self.fVec11[1])));
			self.fRec35[0] = -(self.fConst39 * (self.fConst38 * self.fRec35[1] - (self.fRec36[0] + self.fRec36[1])));
			let mut fTemp31: F32 = self.fRec35[0] * fTemp19;
			let mut fTemp32: F32 = F32::max(0.0, F32::min(fTemp23, self.fConst30 * (1.0 - fTemp23) + 1.0));
			let mut fTemp33: F32 = 1.0 - fTemp32;
			let mut fTemp34: F32 = 0.8 * fTemp24;
			let mut fTemp35: F32 = F32::max(F32::min(0.0005681818 / fTemp13, 0.002), 0.00015);
			let mut fTemp36: F32 = self.fConst0 * fTemp35;
			self.fRec50[0] = if (iTemp22 > 0) as i32 != 0 {fTemp36} else {F32::max(0.0, self.fRec50[1] + -1.0)};
			let mut fTemp37: F32 = F32::max(1.0, fTemp36);
			self.iRec51[0] = i32::wrapping_add(i32::wrapping_mul(i32::wrapping_add(self.iRec51[1], (self.iRec51[1] > 0) as i32), (iTemp22 <= self.iVec9[1]) as i32), (iTemp22 > self.iVec9[1]) as i32);
			let mut fTemp38: F32 = (self.iRec51[0]) as F32;
			let mut fTemp39: F32 = F32::max(0.0, F32::min(fTemp38 / fTemp37, (fTemp37 - fTemp38) / F32::max(1.0, self.fConst40 / fTemp13) + 1.0));
			self.fVec12[0] = fTemp39;
			self.fRec52[0] = if iTemp22 != 0 {self.fVec12[1]} else {self.fRec52[1]};
			let mut fTemp40: F32 = if ((self.fConst17 * (self.fRec50[0] / fTemp35)) > 0.0) as i32 != 0 {fTemp39 + self.fConst17 * (self.fRec50[0] * (self.fRec52[0] - fTemp39) / fTemp35)} else {fTemp39} * (fTemp34 + 0.2);
			self.fVec13[0] = fTemp40;
			self.fRec49[0] = if iTemp21 != 0 {0.33333334 * (self.fVec13[1] - self.fVec13[2])} else {self.fRec49[1]};
			self.fRec53[0] = if iTemp21 != 0 {0.33333334 * self.fVec13[1]} else {self.fRec53[1]};
			let mut fTemp41: F32 = fTemp32 * (self.fRec53[0] + self.fConst27 * self.fRec49[0] * fTemp33);
			let mut iTemp42: i32 = (i32::wrapping_sub(1, self.iVec0[1])) | iTemp21;
			let mut fTemp43: F32 = if iTemp42 != 0 {0.0} else {self.fRec55[1] + self.fConst41 * fTemp13};
			self.fRec55[0] = fTemp43 - F32::floor(fTemp43);
			let mut fTemp44: F32 = if iTemp42 != 0 {0.0} else {self.fRec56[1] + self.fConst42 * fTemp13};
			self.fRec56[0] = fTemp44 - F32::floor(fTemp44);
			let mut fTemp45: F32 = if iTemp42 != 0 {0.0} else {self.fRec57[1] + self.fConst43 * fTemp13};
			self.fRec57[0] = fTemp45 - F32::floor(fTemp45);
			let mut fTemp46: F32 = if iTemp42 != 0 {0.0} else {self.fRec58[1] + self.fConst44 * fTemp13};
			self.fRec58[0] = fTemp46 - F32::floor(fTemp46);
			let mut fTemp47: F32 = (unsafe { ftbl0DSP_BassSIG0[((65536.0 * self.fRec58[0]) as i32) as usize] } + 0.5 * unsafe { ftbl0DSP_BassSIG0[((65536.0 * self.fRec57[0]) as i32) as usize] } + 0.33333334 * unsafe { ftbl0DSP_BassSIG0[((65536.0 * self.fRec56[0]) as i32) as usize] } + 0.25 * unsafe { ftbl0DSP_BassSIG0[((65536.0 * self.fRec55[0]) as i32) as usize] }) * (fTemp41 + fTemp33 * (0.33333334 * fTemp40 - fTemp41));
			let mut fTemp48: F32 = 0.4379562 * fTemp47 + fTemp31;
			self.fRec60[0] = self.fRec5[1];
			let mut fTemp49: F32 = self.fRec25[0] * self.fRec60[1];
			self.fVec15[0] = fTemp49;
			self.fRec63[0] = -(self.fConst22 * (fSlow20 * fTemp49 + fSlow19 * self.fVec15[1] + self.fConst13 * self.fRec63[1]));
			self.fVec16[0] = fSlow21 * self.fRec63[0];
			self.fRec62[0] = -(self.fConst26 * (self.fConst25 * self.fRec62[1] - (fSlow25 * self.fRec63[0] + fSlow24 * self.fVec16[1])));
			self.fRec61[(self.IOTA0 & 511) as usize] = fSlow26 * self.fRec62[0];
			let mut fTemp50: F32 = fSlow34 + fSlow35 * (self.fRec27[0] + -24.0);
			let mut fTemp51: F32 = self.fConst46 * fTemp50;
			let mut fTemp52: F32 = fTemp51 + -1.499995;
			let mut iTemp53: i32 = (fTemp52) as i32;
			let mut iTemp54: i32 = i32::wrapping_add((F32::min(self.fConst45, (std::cmp::max(0, i32::wrapping_add(iTemp53, 4))) as F32)) as i32, 1);
			let mut fTemp55: F32 = F32::floor(fTemp52);
			let mut fTemp56: F32 = fTemp51 + (-3.0 - fTemp55);
			let mut fTemp57: F32 = fTemp51 + (-2.0 - fTemp55);
			let mut fTemp58: F32 = fTemp51 + (-1.0 - fTemp55);
			let mut fTemp59: F32 = fTemp51 - fTemp55;
			let mut fTemp60: F32 = fTemp59 * fTemp58;
			let mut fTemp61: F32 = fTemp60 * fTemp57;
			let mut fTemp62: F32 = fTemp61 * fTemp56;
			let mut iTemp63: i32 = i32::wrapping_add((F32::min(self.fConst45, (std::cmp::max(0, i32::wrapping_add(iTemp53, 3))) as F32)) as i32, 1);
			let mut iTemp64: i32 = i32::wrapping_add((F32::min(self.fConst45, (std::cmp::max(0, i32::wrapping_add(iTemp53, 2))) as F32)) as i32, 1);
			let mut iTemp65: i32 = i32::wrapping_add((F32::min(self.fConst45, (std::cmp::max(0, i32::wrapping_add(iTemp53, 1))) as F32)) as i32, 1);
			let mut iTemp66: i32 = i32::wrapping_add((F32::min(self.fConst45, (std::cmp::max(0, iTemp53)) as F32)) as i32, 1);
			let mut fTemp67: F32 = fTemp51 + (-4.0 - fTemp55);
			self.fVec17[0] = fTemp67 * (fTemp56 * (fTemp57 * (0.041666668 * self.fRec61[((i32::wrapping_sub(self.IOTA0, iTemp66)) & 511) as usize] * fTemp58 - 0.16666667 * fTemp59 * self.fRec61[((i32::wrapping_sub(self.IOTA0, iTemp65)) & 511) as usize]) + 0.25 * fTemp60 * self.fRec61[((i32::wrapping_sub(self.IOTA0, iTemp64)) & 511) as usize]) - 0.16666667 * fTemp61 * self.fRec61[((i32::wrapping_sub(self.IOTA0, iTemp63)) & 511) as usize]) + 0.041666668 * fTemp62 * self.fRec61[((i32::wrapping_sub(self.IOTA0, iTemp54)) & 511) as usize];
			let mut fTemp68: F32 = self.fVec17[1] + self.fRec11[1];
			let mut fTemp69: F32 = F32::abs(fTemp68);
			self.fRec59[0] = F32::max(fTemp69, self.fConst14 * self.fRec59[1] + self.fConst47 * fTemp69);
			let mut fTemp70: F32 = F32::min(1.0, F32::max(0.0, 5.0 * (fTemp34 + (0.3 - 3.0 * self.fRec59[0]))));
			let mut fTemp71: F32 = fTemp70 * fTemp48;
			let mut fTemp72: F32 = self.fVec17[1] + fTemp71;
			self.fVec18[(self.IOTA0 & 8191) as usize] = fTemp72;
			let mut fTemp73: F32 = self.fConst0 * (0.0011363636 / fTemp13 - 2.9411765e-05 * fTemp50);
			let mut fTemp74: F32 = fSlow36 + fTemp73 + -6.549995;
			let mut iTemp75: i32 = (fTemp74) as i32;
			let mut iTemp76: i32 = i32::wrapping_add((F32::min(self.fConst45, (std::cmp::max(0, i32::wrapping_add(iTemp75, 4))) as F32)) as i32, 1);
			let mut fTemp77: F32 = F32::floor(fTemp74);
			let mut fTemp78: F32 = fSlow36 + fTemp73 + (-8.05 - fTemp77);
			let mut fTemp79: F32 = fSlow36 + fTemp73 + (-7.05 - fTemp77);
			let mut fTemp80: F32 = fSlow36 + fTemp73 + (-6.05 - fTemp77);
			let mut fTemp81: F32 = fSlow36 + fTemp73 + (-5.05 - fTemp77);
			let mut fTemp82: F32 = fTemp81 * fTemp80;
			let mut fTemp83: F32 = fTemp82 * fTemp79;
			let mut fTemp84: F32 = fTemp83 * fTemp78;
			let mut iTemp85: i32 = i32::wrapping_add((F32::min(self.fConst45, (std::cmp::max(0, i32::wrapping_add(iTemp75, 3))) as F32)) as i32, 1);
			let mut iTemp86: i32 = i32::wrapping_add((F32::min(self.fConst45, (std::cmp::max(0, i32::wrapping_add(iTemp75, 2))) as F32)) as i32, 1);
			let mut iTemp87: i32 = i32::wrapping_add((F32::min(self.fConst45, (std::cmp::max(0, i32::wrapping_add(iTemp75, 1))) as F32)) as i32, 1);
			let mut iTemp88: i32 = i32::wrapping_add((F32::min(self.fConst45, (std::cmp::max(0, iTemp75)) as F32)) as i32, 1);
			let mut fTemp89: F32 = fSlow36 + fTemp73 + (-9.05 - fTemp77);
			self.fRec20[0] = fTemp89 * (fTemp78 * (fTemp79 * (0.041666668 * self.fVec18[((i32::wrapping_sub(self.IOTA0, iTemp88)) & 8191) as usize] * fTemp80 - 0.16666667 * fTemp81 * self.fVec18[((i32::wrapping_sub(self.IOTA0, iTemp87)) & 8191) as usize]) + 0.25 * fTemp82 * self.fVec18[((i32::wrapping_sub(self.IOTA0, iTemp86)) & 8191) as usize]) - 0.16666667 * fTemp83 * self.fVec18[((i32::wrapping_sub(self.IOTA0, iTemp85)) & 8191) as usize]) + 0.041666668 * fTemp84 * self.fVec18[((i32::wrapping_sub(self.IOTA0, iTemp76)) & 8191) as usize];
			let mut fRec21: F32 = fTemp68 + fTemp19 * fTemp70 * (fTemp31 + fTemp48 + 0.4379562 * fTemp47 * (fSlow37 + fSlow38 * self.fRec33[0]) * (1.0 - DSP_Bass_faustpower2_f(1.0 - fTemp24)));
			self.fRec15[(self.IOTA0 & 8191) as usize] = fRec19;
			let mut fRec16: F32 = fTemp89 * (fTemp78 * (fTemp79 * (0.041666668 * fTemp80 * self.fRec15[((i32::wrapping_sub(self.IOTA0, iTemp88)) & 8191) as usize] - 0.16666667 * fTemp81 * self.fRec15[((i32::wrapping_sub(self.IOTA0, iTemp87)) & 8191) as usize]) + 0.25 * fTemp82 * self.fRec15[((i32::wrapping_sub(self.IOTA0, iTemp86)) & 8191) as usize]) - 0.16666667 * fTemp83 * self.fRec15[((i32::wrapping_sub(self.IOTA0, iTemp85)) & 8191) as usize]) + 0.041666668 * fTemp84 * self.fRec15[((i32::wrapping_sub(self.IOTA0, iTemp76)) & 8191) as usize];
			let mut fRec17: F32 = self.fRec20[0];
			let mut fRec18: F32 = fRec21;
			self.fRec11[0] = fRec16;
			let mut fRec12: F32 = self.fRec11[1] + fTemp71;
			let mut fRec13: F32 = fRec17;
			let mut fRec14: F32 = fRec18;
			self.fRec7[(self.IOTA0 & 511) as usize] = fRec12;
			let mut fRec8: F32 = fTemp67 * (fTemp56 * (fTemp57 * (0.041666668 * fTemp58 * self.fRec7[((i32::wrapping_sub(self.IOTA0, iTemp66)) & 511) as usize] - 0.16666667 * fTemp59 * self.fRec7[((i32::wrapping_sub(self.IOTA0, iTemp65)) & 511) as usize]) + 0.25 * fTemp60 * self.fRec7[((i32::wrapping_sub(self.IOTA0, iTemp64)) & 511) as usize]) - 0.16666667 * fTemp61 * self.fRec7[((i32::wrapping_sub(self.IOTA0, iTemp63)) & 511) as usize]) + 0.041666668 * fTemp62 * self.fRec7[((i32::wrapping_sub(self.IOTA0, iTemp54)) & 511) as usize];
			let mut fRec9: F32 = fRec13;
			let mut fRec10: F32 = fRec14;
			self.fRec5[0] = fRec8;
			self.fRec6[0] = fRec10;
			self.fRec4[0] = -(self.fConst22 * (self.fConst13 * self.fRec4[1] - (self.fConst51 * self.fRec6[0] + self.fConst50 * self.fRec6[1])));
			self.fRec3[0] = -(self.fConst26 * (self.fConst25 * self.fRec3[1] - self.fConst56 * (self.fConst55 * self.fRec4[0] + self.fConst54 * self.fRec4[1])));
			self.fRec2[0] = self.fConst57 * self.fRec3[0] - self.fConst11 * (self.fConst9 * self.fRec2[2] + fTemp0);
			let mut fTemp90: F32 = fTemp0 + self.fConst60 * self.fRec2[0] + self.fConst59 * self.fRec2[2];
			self.fVec19[0] = fTemp90;
			self.fRec1[0] = fSlow39 * (self.fConst61 * (fTemp90 + self.fVec19[1]) - fSlow1 * self.fRec1[1]);
			self.fRec0[0] = self.fConst62 * (self.fRec1[0] - self.fRec1[1] + self.fConst2 * self.fRec0[1]);
			*output0 = self.fRec0[0];
			self.iVec0[1] = self.iVec0[0];
			self.fVec1[1] = self.fVec1[0];
			self.fRec26[1] = self.fRec26[0];
			self.fRec28[1] = self.fRec28[0];
			self.fRec30[1] = self.fRec30[0];
			self.fRec29[1] = self.fRec29[0];
			self.fRec31[1] = self.fRec31[0];
			self.fVec2[1] = self.fVec2[0];
			self.fVec3[1] = self.fVec3[0];
			self.fRec32[1] = self.fRec32[0];
			self.fRec27[1] = self.fRec27[0];
			self.fRec33[1] = self.fRec33[0];
			self.fRec34[1] = self.fRec34[0];
			self.fVec4[1] = self.fVec4[0];
			self.fRec24[1] = self.fRec24[0];
			self.fRec25[1] = self.fRec25[0];
			self.fVec5[1] = self.fVec5[0];
			self.fRec23[1] = self.fRec23[0];
			self.fVec6[1] = self.fVec6[0];
			self.fRec22[1] = self.fRec22[0];
			self.iVec7[1] = self.iVec7[0];
			self.iVec8[1] = self.iVec8[0];
			self.fRec37[1] = self.fRec37[0];
			self.iVec9[1] = self.iVec9[0];
			self.iRec38[1] = self.iRec38[0];
			self.fRec39[1] = self.fRec39[0];
			self.fRec40[1] = self.fRec40[0];
			self.fRec41[1] = self.fRec41[0];
			self.fRec42[1] = self.fRec42[0];
			self.fVec10[1] = self.fVec10[0];
			self.fRec43[1] = self.fRec43[0];
			self.iRec47[1] = self.iRec47[0];
			self.iRec48[1] = self.iRec48[0];
			self.fRec46[1] = self.fRec46[0];
			self.fRec44[1] = self.fRec44[0];
			self.fRec45[1] = self.fRec45[0];
			self.fVec11[1] = self.fVec11[0];
			self.fRec36[1] = self.fRec36[0];
			self.fRec35[1] = self.fRec35[0];
			self.fRec50[1] = self.fRec50[0];
			self.iRec51[1] = self.iRec51[0];
			self.fVec12[1] = self.fVec12[0];
			self.fRec52[1] = self.fRec52[0];
			self.fVec13[2] = self.fVec13[1];
			self.fVec13[1] = self.fVec13[0];
			self.fRec49[1] = self.fRec49[0];
			self.fRec53[1] = self.fRec53[0];
			self.fRec55[1] = self.fRec55[0];
			self.fRec56[1] = self.fRec56[0];
			self.fRec57[1] = self.fRec57[0];
			self.fRec58[1] = self.fRec58[0];
			self.fRec60[1] = self.fRec60[0];
			self.fVec15[1] = self.fVec15[0];
			self.fRec63[1] = self.fRec63[0];
			self.fVec16[1] = self.fVec16[0];
			self.fRec62[1] = self.fRec62[0];
			self.IOTA0 = i32::wrapping_add(self.IOTA0, 1);
			self.fVec17[1] = self.fVec17[0];
			self.fRec59[1] = self.fRec59[0];
			self.fRec20[1] = self.fRec20[0];
			self.fRec11[1] = self.fRec11[0];
			self.fRec5[1] = self.fRec5[0];
			self.fRec6[1] = self.fRec6[0];
			self.fRec4[1] = self.fRec4[0];
			self.fRec3[1] = self.fRec3[0];
			self.fRec2[2] = self.fRec2[1];
			self.fRec2[1] = self.fRec2[0];
			self.fVec19[1] = self.fVec19[0];
			self.fRec1[1] = self.fRec1[0];
			self.fRec0[1] = self.fRec0[0];
		}
	}

}

