/* ------------------------------------------------------------
author: "Punk Labs LLC"
copyright: "2024 Punk Labs LLC"
name: "OneTrick CHONK Bass"
version: "1.0"
Code generated with Faust 2.88.0 (https://faust.grame.fr)
Compilation options: -lang cpp -fpga-mem-th 4 -ct 0 -cn DSP_Bass -dtl 65536 -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __DSP_Bass_H__
#define  __DSP_Bass_H__

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif 

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <math.h>
#ifndef FAUST_INT_WRAP
#define FAUST_INT_WRAP
inline int faust_wrap_add(int a, int b) { return int((unsigned int)a + (unsigned int)b); }
inline int faust_wrap_sub(int a, int b) { return int((unsigned int)a - (unsigned int)b); }
inline int faust_wrap_mul(int a, int b) { return int((unsigned int)a * (unsigned int)b); }
#endif


#ifndef FAUSTCLASS 
#define FAUSTCLASS DSP_Bass
#endif

#ifdef __APPLE__ 
#define exp10f __exp10f
#define exp10 __exp10
#endif

#if defined(_WIN32)
#define RESTRICT __restrict
#else
#define RESTRICT __restrict__
#endif

class DSP_BassSIG0 {
	
  private:
	
	int iVec10[2];
	int iRec32[2];
	int fSampleRate;
	
  public:
	
	int getNumInputsDSP_BassSIG0() {
		return 0;
	}
	int getNumOutputsDSP_BassSIG0() {
		return 1;
	}
	
	void instanceInitDSP_BassSIG0(int sample_rate) {
		fSampleRate = sample_rate;
		for (int l35 = 0; l35 < 2; l35 = faust_wrap_add(l35, 1)) {
			iVec10[l35] = 0;
		}
		for (int l36 = 0; l36 < 2; l36 = faust_wrap_add(l36, 1)) {
			iRec32[l36] = 0;
		}
	}
	
	void fillDSP_BassSIG0(int count, float* table) {
		for (int i1 = 0; i1 < count; i1 = faust_wrap_add(i1, 1)) {
			iVec10[0] = 1;
			iRec32[0] = (faust_wrap_add(iVec10[1], iRec32[1])) % 65536;
			table[i1] = std::sin(9.58738e-05f * static_cast<float>(iRec32[0]));
			iVec10[1] = iVec10[0];
			iRec32[1] = iRec32[0];
		}
	}

};

static DSP_BassSIG0* newDSP_BassSIG0() { return (DSP_BassSIG0*)new DSP_BassSIG0(); }
static void deleteDSP_BassSIG0(DSP_BassSIG0* dsp) { delete dsp; }

static float DSP_Bass_faustpower2_f(float value) {
	return value * value;
}
static float ftbl0DSP_BassSIG0[65536];

class DSP_Bass : public dsp {
	
 private:
	
	int fSampleRate;
	float fConst0;
	float fConst1;
	float fConst2;
	float fConst3;
	FAUSTFLOAT fHslider0;
	float fConst4;
	float fConst5;
	float fConst6;
	float fConst7;
	float fConst8;
	float fConst9;
	float fConst10;
	float fConst11;
	float fConst12;
	float fConst13;
	float fConst14;
	float fConst15;
	float fConst16;
	float fConst17;
	float fConst18;
	float fConst19;
	int iVec0[2];
	float fConst20;
	float fConst21;
	float fConst22;
	float fConst23;
	float fConst24;
	float fConst25;
	float fConst26;
	float fConst27;
	float fConst28;
	FAUSTFLOAT fHslider1;
	FAUSTFLOAT fHslider2;
	FAUSTFLOAT fButton0;
	float fConst29;
	FAUSTFLOAT fButton1;
	float fVec1[2];
	float fRec18[2];
	FAUSTFLOAT fHslider3;
	FAUSTFLOAT fEntry0;
	float fConst30;
	FAUSTFLOAT fEntry1;
	FAUSTFLOAT fHslider4;
	float fRec20[2];
	FAUSTFLOAT fEntry2;
	FAUSTFLOAT fHslider5;
	FAUSTFLOAT fButton2;
	float fRec22[2];
	FAUSTFLOAT fButton3;
	float fRec23[2];
	float fConst31;
	float fRec21[2];
	FAUSTFLOAT fEntry3;
	FAUSTFLOAT fEntry4;
	FAUSTFLOAT fEntry5;
	float fVec2[2];
	float fVec3[2];
	float fRec24[2];
	FAUSTFLOAT fButton4;
	float fRec25[2];
	FAUSTFLOAT fEntry6;
	FAUSTFLOAT fHslider6;
	FAUSTFLOAT fEntry7;
	float fConst32;
	float fRec26[2];
	float fRec19[2];
	FAUSTFLOAT fHslider7;
	float fRec27[2];
	FAUSTFLOAT fButton5;
	float fRec28[2];
	FAUSTFLOAT fButton6;
	float fRec29[2];
	float fVec4[2];
	float fConst33;
	float fRec30[2];
	float fRec17[2];
	float fVec5[2];
	float fRec5[2];
	float fVec6[2];
	float fRec6[2];
	float fRec7[2];
	int IOTA0;
	float fRec8[8192];
	float fVec7[2];
	float fRec9[2];
	float fConst34;
	FAUSTFLOAT fHslider8;
	float fConst35;
	float fRec10[2];
	float fRec11[2];
	float fVec8[2];
	float fRec12[2];
	float fVec9[2];
	float fConst36;
	float fRec13[2];
	float fRec14[8192];
	FAUSTFLOAT fEntry8;
	FAUSTFLOAT fEntry9;
	float fRec31[2];
	int iVec11[2];
	int iVec12[2];
	float fConst37;
	float fRec33[2];
	float fConst38;
	float fRec34[2];
	float fConst39;
	float fRec35[2];
	float fConst40;
	float fRec36[2];
	int iVec13[2];
	int iRec37[2];
	float fConst41;
	float fConst42;
	float fConst43;
	FAUSTFLOAT fHslider9;
	float fRec39[2];
	int iRec40[2];
	float fConst44;
	float fVec14[2];
	float fRec41[2];
	float fVec15[3];
	float fRec38[2];
	float fRec42[2];
	float fConst45;
	int iRec47[2];
	float fConst46;
	int iConst47;
	int iRec48[2];
	float fRec46[2];
	float fRec49[2];
	float fRec45[2];
	FAUSTFLOAT fHslider10;
	FAUSTFLOAT fButton7;
	float fRec50[2];
	FAUSTFLOAT fButton8;
	float fRec51[2];
	FAUSTFLOAT fButton9;
	float fRec52[2];
	float fConst48;
	float fRec53[2];
	float fConst49;
	float fVec16[2];
	float fRec54[2];
	float fVec17[2];
	float fRec44[2];
	float fRec43[2];
	float fVec18[8192];
	float fRec15[2];
	float fRec16[8192];
	FAUSTFLOAT fHslider11;
	float fVec19[2];
	float fConst50;
	float fRec4[2];
	float fConst51;
	float fRec3[2];
	float fConst52;
	float fConst53;
	float fRec2[3];
	float fConst54;
	float fConst55;
	float fConst56;
	float fVec20[2];
	float fRec1[2];
	float fConst57;
	float fRec0[2];
	
 public:
	DSP_Bass() {
	}
	
	DSP_Bass(const DSP_Bass&) = default;
	
	virtual ~DSP_Bass() = default;
	
	DSP_Bass& operator=(const DSP_Bass&) = default;
	
	void metadata(Meta* m) { 
		m->declare("analyzers.lib/name", "Faust Analyzer Library");
		m->declare("analyzers.lib/version", "1.4.0");
		m->declare("author", "Punk Labs LLC");
		m->declare("basics.lib/downSample:author", "Romain Michon");
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/sAndH:author", "Romain Michon");
		m->declare("basics.lib/version", "1.23.0");
		m->declare("compile_options", "-lang cpp -fpga-mem-th 4 -ct 0 -cn DSP_Bass -dtl 65536 -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0");
		m->declare("copyright", "2024 Punk Labs LLC");
		m->declare("delays.lib/fdelay4:author", "Julius O. Smith III");
		m->declare("delays.lib/fdelayltv:author", "Julius O. Smith III");
		m->declare("delays.lib/name", "Faust Delay Library");
		m->declare("delays.lib/version", "1.2.0");
		m->declare("envelopes.lib/ar:author", "Yann Orlarey, Stéphane Letz");
		m->declare("envelopes.lib/author", "GRAME");
		m->declare("envelopes.lib/copyright", "GRAME");
		m->declare("envelopes.lib/license", "LicenseRef-LGPL-2.1-or-later-with-Faust-exception");
		m->declare("envelopes.lib/name", "Faust Envelope Library");
		m->declare("envelopes.lib/version", "1.3.0");
		m->declare("filename", "bass.dsp");
		m->declare("filters.lib/dcblockerat:author", "Julius O. Smith III");
		m->declare("filters.lib/dcblockerat:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/dcblockerat:license", "LicenseRef-STK-4.3");
		m->declare("filters.lib/fir:author", "Julius O. Smith III");
		m->declare("filters.lib/fir:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/fir:license", "LicenseRef-STK-4.3");
		m->declare("filters.lib/highpass:author", "Julius O. Smith III");
		m->declare("filters.lib/highpass:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/highpass:license", "LicenseRef-STK-4.3");
		m->declare("filters.lib/iir:author", "Julius O. Smith III");
		m->declare("filters.lib/iir:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/iir:license", "LicenseRef-STK-4.3");
		m->declare("filters.lib/lowpass0_highpass1:author", "Julius O. Smith III");
		m->declare("filters.lib/lowpass0_highpass1:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/lowpass0_highpass1:license", "LicenseRef-STK-4.3");
		m->declare("filters.lib/lowpass:author", "Julius O. Smith III");
		m->declare("filters.lib/lowpass:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/lowpass:license", "LicenseRef-STK-4.3");
		m->declare("filters.lib/name", "Faust Filters Library");
		m->declare("filters.lib/peak_eq:author", "Julius O. Smith III");
		m->declare("filters.lib/peak_eq:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/peak_eq:license", "LicenseRef-STK-4.3");
		m->declare("filters.lib/peak_eq_cq:author", "Julius O. Smith III");
		m->declare("filters.lib/peak_eq_cq:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/peak_eq_cq:license", "LicenseRef-STK-4.3");
		m->declare("filters.lib/pole:author", "Julius O. Smith III");
		m->declare("filters.lib/pole:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/pole:license", "LicenseRef-STK-4.3");
		m->declare("filters.lib/spectral_tilt:author", "Julius O. Smith III");
		m->declare("filters.lib/spectral_tilt:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/spectral_tilt:license", "LicenseRef-STK-4.3");
		m->declare("filters.lib/tf1:author", "Julius O. Smith III");
		m->declare("filters.lib/tf1:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf1:license", "LicenseRef-STK-4.3");
		m->declare("filters.lib/tf1s:author", "Julius O. Smith III");
		m->declare("filters.lib/tf1s:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf1s:license", "LicenseRef-STK-4.3");
		m->declare("filters.lib/tf2:author", "Julius O. Smith III");
		m->declare("filters.lib/tf2:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf2:license", "LicenseRef-STK-4.3");
		m->declare("filters.lib/tf2s:author", "Julius O. Smith III");
		m->declare("filters.lib/tf2s:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf2s:license", "LicenseRef-STK-4.3");
		m->declare("filters.lib/version", "1.9.0");
		m->declare("filters.lib/zero:author", "Julius O. Smith III");
		m->declare("filters.lib/zero:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/zero:license", "LicenseRef-STK-4.3");
		m->declare("interpolators.lib/interpolate_linear:author", "Stéphane Letz");
		m->declare("interpolators.lib/interpolate_linear:licence", "MIT");
		m->declare("interpolators.lib/name", "Faust Interpolator Library");
		m->declare("interpolators.lib/remap:author", "David Braun");
		m->declare("interpolators.lib/version", "1.6.0");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LicenseRef-LGPL-2.1-or-later-with-Faust-exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.9.0");
		m->declare("name", "OneTrick CHONK Bass");
		m->declare("noises.lib/name", "Faust Noise Generator Library");
		m->declare("noises.lib/version", "1.6.0");
		m->declare("onetrick.lib/copyright", "Copyright (c) 2023 Punk Labs LLC");
		m->declare("onetrick.lib/license", "GPLv3 (or later)");
		m->declare("onetrick.lib/name", "OneTrick DSP Library");
		m->declare("oscillators.lib/hs_oscsin:author", "Mike Olsen");
		m->declare("oscillators.lib/hs_phasor:author", "Mike Olsen, revised by Stéphane Letz");
		m->declare("oscillators.lib/name", "Faust Oscillator Library");
		m->declare("oscillators.lib/version", "1.8.0");
		m->declare("physmodels.lib/name", "Faust Physical Models Library");
		m->declare("physmodels.lib/version", "1.2.0");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "1.3.0");
		m->declare("routes.lib/name", "Faust Signal Routing Library");
		m->declare("routes.lib/version", "1.4.0");
		m->declare("signals.lib/name", "Faust Routing Library");
		m->declare("signals.lib/version", "1.7.0");
		m->declare("version", "1.0");
	}

	virtual int getNumInputs() {
		return 0;
	}
	virtual int getNumOutputs() {
		return 1;
	}
	
	static void classInit(int sample_rate) {
		DSP_BassSIG0* sig0 = newDSP_BassSIG0();
		sig0->instanceInitDSP_BassSIG0(sample_rate);
		sig0->fillDSP_BassSIG0(65536, ftbl0DSP_BassSIG0);
		deleteDSP_BassSIG0(sig0);
	}
	
	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		fConst0 = std::min<float>(1.92e+05f, std::max<float>(1.0f, static_cast<float>(fSampleRate)));
		fConst1 = 62.831852f / fConst0;
		fConst2 = 1.0f / (fConst1 + 1.0f);
		fConst3 = 1382.3008f / fConst0;
		fConst4 = std::tan(97.38937f / fConst0);
		fConst5 = 1.0f / fConst4;
		fConst6 = fConst0 * std::sin(194.77875f / fConst0);
		fConst7 = 194.77875f / fConst6;
		fConst8 = (fConst5 + fConst7) / fConst4 + 1.0f;
		fConst9 = 0.70794576f / fConst8;
		fConst10 = 2.0f * (1.0f - 1.0f / DSP_Bass_faustpower2_f(fConst4));
		fConst11 = std::tan(20420.352f / fConst0);
		fConst12 = std::tan(46985.598f / fConst0);
		fConst13 = fConst11 / fConst12;
		fConst14 = 1.0f / std::tan(0.5f / fConst0);
		fConst15 = 6283.1855f / fConst0;
		fConst16 = std::tan(fConst15);
		fConst17 = 12566.371f * (fConst11 / fConst16);
		fConst18 = 1.0f / (fConst14 + fConst17);
		fConst19 = fConst17 - fConst14;
		fConst20 = std::tan(14457.107f / fConst0);
		fConst21 = fConst16 / fConst20;
		fConst22 = 12566.371f * (fConst12 / fConst16);
		fConst23 = fConst14 + fConst22;
		fConst24 = 1.0f / (fConst14 + 12566.371f);
		fConst25 = 12566.371f - fConst14;
		fConst26 = 12566.371f * (fConst20 / fConst16);
		fConst27 = fConst14 + fConst26;
		fConst28 = 12566.371f / fConst16;
		fConst29 = std::exp(-(1e+03f / fConst0));
		fConst30 = std::exp(-(5e+02f / fConst0));
		fConst31 = 1.0f / fConst0;
		fConst32 = std::exp(-(1e+02f / fConst0));
		fConst33 = 0.002f * fConst0;
		fConst34 = 2.9411765e-05f * fConst0;
		fConst35 = 0.03057805f * fConst0;
		fConst36 = 1.0f - fConst32;
		fConst37 = 4.4e+02f / fConst0;
		fConst38 = 8.8e+02f / fConst0;
		fConst39 = 1.32e+03f / fConst0;
		fConst40 = 1.76e+03f / fConst0;
		fConst41 = 0.0002f * fConst0;
		fConst42 = std::max<float>(1.0f, fConst41);
		fConst43 = 1.0f / fConst42;
		fConst44 = 0.0045454544f * fConst0;
		fConst45 = 3.1415927f / fConst0;
		fConst46 = fConst0 / std::min<float>(std::min<float>(4.8e+04f, fConst0), fConst0);
		iConst47 = static_cast<int>(fConst46);
		fConst48 = 5e+03f / fConst0;
		fConst49 = 0.001f * fConst0;
		fConst50 = fConst26 - fConst14;
		fConst51 = fConst22 - fConst14;
		fConst52 = 1.0f / fConst8;
		fConst53 = (fConst5 - fConst7) / fConst4 + 1.0f;
		fConst54 = 388.63467f / fConst6;
		fConst55 = (fConst5 + fConst54) / fConst4 + 1.0f;
		fConst56 = (fConst5 - fConst54) / fConst4 + 1.0f;
		fConst57 = 1.0f - fConst1;
	}
	
	virtual void instanceResetUserInterface() {
		fHslider0 = static_cast<FAUSTFLOAT>(5e+01f);
		fHslider1 = static_cast<FAUSTFLOAT>(5e+01f);
		fHslider2 = static_cast<FAUSTFLOAT>(0.0f);
		fButton0 = static_cast<FAUSTFLOAT>(0.0f);
		fButton1 = static_cast<FAUSTFLOAT>(0.0f);
		fHslider3 = static_cast<FAUSTFLOAT>(1e+02f);
		fEntry0 = static_cast<FAUSTFLOAT>(0.0f);
		fEntry1 = static_cast<FAUSTFLOAT>(0.0f);
		fHslider4 = static_cast<FAUSTFLOAT>(1.0f);
		fEntry2 = static_cast<FAUSTFLOAT>(36.0f);
		fHslider5 = static_cast<FAUSTFLOAT>(35.0f);
		fButton2 = static_cast<FAUSTFLOAT>(0.0f);
		fButton3 = static_cast<FAUSTFLOAT>(0.0f);
		fEntry3 = static_cast<FAUSTFLOAT>(0.0f);
		fEntry4 = static_cast<FAUSTFLOAT>(0.0f);
		fEntry5 = static_cast<FAUSTFLOAT>(0.0f);
		fButton4 = static_cast<FAUSTFLOAT>(0.0f);
		fEntry6 = static_cast<FAUSTFLOAT>(0.0f);
		fHslider6 = static_cast<FAUSTFLOAT>(2.0f);
		fEntry7 = static_cast<FAUSTFLOAT>(0.0f);
		fHslider7 = static_cast<FAUSTFLOAT>(0.0f);
		fButton5 = static_cast<FAUSTFLOAT>(0.0f);
		fButton6 = static_cast<FAUSTFLOAT>(0.0f);
		fHslider8 = static_cast<FAUSTFLOAT>(5e+01f);
		fEntry8 = static_cast<FAUSTFLOAT>(0.0f);
		fEntry9 = static_cast<FAUSTFLOAT>(0.0f);
		fHslider9 = static_cast<FAUSTFLOAT>(1e+02f);
		fHslider10 = static_cast<FAUSTFLOAT>(0.0f);
		fButton7 = static_cast<FAUSTFLOAT>(0.0f);
		fButton8 = static_cast<FAUSTFLOAT>(0.0f);
		fButton9 = static_cast<FAUSTFLOAT>(0.0f);
		fHslider11 = static_cast<FAUSTFLOAT>(5e+01f);
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; l0 < 2; l0 = faust_wrap_add(l0, 1)) {
			iVec0[l0] = 0;
		}
		for (int l1 = 0; l1 < 2; l1 = faust_wrap_add(l1, 1)) {
			fVec1[l1] = 0.0f;
		}
		for (int l2 = 0; l2 < 2; l2 = faust_wrap_add(l2, 1)) {
			fRec18[l2] = 0.0f;
		}
		for (int l3 = 0; l3 < 2; l3 = faust_wrap_add(l3, 1)) {
			fRec20[l3] = 0.0f;
		}
		for (int l4 = 0; l4 < 2; l4 = faust_wrap_add(l4, 1)) {
			fRec22[l4] = 0.0f;
		}
		for (int l5 = 0; l5 < 2; l5 = faust_wrap_add(l5, 1)) {
			fRec23[l5] = 0.0f;
		}
		for (int l6 = 0; l6 < 2; l6 = faust_wrap_add(l6, 1)) {
			fRec21[l6] = 0.0f;
		}
		for (int l7 = 0; l7 < 2; l7 = faust_wrap_add(l7, 1)) {
			fVec2[l7] = 0.0f;
		}
		for (int l8 = 0; l8 < 2; l8 = faust_wrap_add(l8, 1)) {
			fVec3[l8] = 0.0f;
		}
		for (int l9 = 0; l9 < 2; l9 = faust_wrap_add(l9, 1)) {
			fRec24[l9] = 0.0f;
		}
		for (int l10 = 0; l10 < 2; l10 = faust_wrap_add(l10, 1)) {
			fRec25[l10] = 0.0f;
		}
		for (int l11 = 0; l11 < 2; l11 = faust_wrap_add(l11, 1)) {
			fRec26[l11] = 0.0f;
		}
		for (int l12 = 0; l12 < 2; l12 = faust_wrap_add(l12, 1)) {
			fRec19[l12] = 0.0f;
		}
		for (int l13 = 0; l13 < 2; l13 = faust_wrap_add(l13, 1)) {
			fRec27[l13] = 0.0f;
		}
		for (int l14 = 0; l14 < 2; l14 = faust_wrap_add(l14, 1)) {
			fRec28[l14] = 0.0f;
		}
		for (int l15 = 0; l15 < 2; l15 = faust_wrap_add(l15, 1)) {
			fRec29[l15] = 0.0f;
		}
		for (int l16 = 0; l16 < 2; l16 = faust_wrap_add(l16, 1)) {
			fVec4[l16] = 0.0f;
		}
		for (int l17 = 0; l17 < 2; l17 = faust_wrap_add(l17, 1)) {
			fRec30[l17] = 0.0f;
		}
		for (int l18 = 0; l18 < 2; l18 = faust_wrap_add(l18, 1)) {
			fRec17[l18] = 0.0f;
		}
		for (int l19 = 0; l19 < 2; l19 = faust_wrap_add(l19, 1)) {
			fVec5[l19] = 0.0f;
		}
		for (int l20 = 0; l20 < 2; l20 = faust_wrap_add(l20, 1)) {
			fRec5[l20] = 0.0f;
		}
		for (int l21 = 0; l21 < 2; l21 = faust_wrap_add(l21, 1)) {
			fVec6[l21] = 0.0f;
		}
		for (int l22 = 0; l22 < 2; l22 = faust_wrap_add(l22, 1)) {
			fRec6[l22] = 0.0f;
		}
		for (int l23 = 0; l23 < 2; l23 = faust_wrap_add(l23, 1)) {
			fRec7[l23] = 0.0f;
		}
		IOTA0 = 0;
		for (int l24 = 0; l24 < 8192; l24 = faust_wrap_add(l24, 1)) {
			fRec8[l24] = 0.0f;
		}
		for (int l25 = 0; l25 < 2; l25 = faust_wrap_add(l25, 1)) {
			fVec7[l25] = 0.0f;
		}
		for (int l26 = 0; l26 < 2; l26 = faust_wrap_add(l26, 1)) {
			fRec9[l26] = 0.0f;
		}
		for (int l27 = 0; l27 < 2; l27 = faust_wrap_add(l27, 1)) {
			fRec10[l27] = 0.0f;
		}
		for (int l28 = 0; l28 < 2; l28 = faust_wrap_add(l28, 1)) {
			fRec11[l28] = 0.0f;
		}
		for (int l29 = 0; l29 < 2; l29 = faust_wrap_add(l29, 1)) {
			fVec8[l29] = 0.0f;
		}
		for (int l30 = 0; l30 < 2; l30 = faust_wrap_add(l30, 1)) {
			fRec12[l30] = 0.0f;
		}
		for (int l31 = 0; l31 < 2; l31 = faust_wrap_add(l31, 1)) {
			fVec9[l31] = 0.0f;
		}
		for (int l32 = 0; l32 < 2; l32 = faust_wrap_add(l32, 1)) {
			fRec13[l32] = 0.0f;
		}
		for (int l33 = 0; l33 < 8192; l33 = faust_wrap_add(l33, 1)) {
			fRec14[l33] = 0.0f;
		}
		for (int l34 = 0; l34 < 2; l34 = faust_wrap_add(l34, 1)) {
			fRec31[l34] = 0.0f;
		}
		for (int l37 = 0; l37 < 2; l37 = faust_wrap_add(l37, 1)) {
			iVec11[l37] = 0;
		}
		for (int l38 = 0; l38 < 2; l38 = faust_wrap_add(l38, 1)) {
			iVec12[l38] = 0;
		}
		for (int l39 = 0; l39 < 2; l39 = faust_wrap_add(l39, 1)) {
			fRec33[l39] = 0.0f;
		}
		for (int l40 = 0; l40 < 2; l40 = faust_wrap_add(l40, 1)) {
			fRec34[l40] = 0.0f;
		}
		for (int l41 = 0; l41 < 2; l41 = faust_wrap_add(l41, 1)) {
			fRec35[l41] = 0.0f;
		}
		for (int l42 = 0; l42 < 2; l42 = faust_wrap_add(l42, 1)) {
			fRec36[l42] = 0.0f;
		}
		for (int l43 = 0; l43 < 2; l43 = faust_wrap_add(l43, 1)) {
			iVec13[l43] = 0;
		}
		for (int l44 = 0; l44 < 2; l44 = faust_wrap_add(l44, 1)) {
			iRec37[l44] = 0;
		}
		for (int l45 = 0; l45 < 2; l45 = faust_wrap_add(l45, 1)) {
			fRec39[l45] = 0.0f;
		}
		for (int l46 = 0; l46 < 2; l46 = faust_wrap_add(l46, 1)) {
			iRec40[l46] = 0;
		}
		for (int l47 = 0; l47 < 2; l47 = faust_wrap_add(l47, 1)) {
			fVec14[l47] = 0.0f;
		}
		for (int l48 = 0; l48 < 2; l48 = faust_wrap_add(l48, 1)) {
			fRec41[l48] = 0.0f;
		}
		for (int l49 = 0; l49 < 3; l49 = faust_wrap_add(l49, 1)) {
			fVec15[l49] = 0.0f;
		}
		for (int l50 = 0; l50 < 2; l50 = faust_wrap_add(l50, 1)) {
			fRec38[l50] = 0.0f;
		}
		for (int l51 = 0; l51 < 2; l51 = faust_wrap_add(l51, 1)) {
			fRec42[l51] = 0.0f;
		}
		for (int l52 = 0; l52 < 2; l52 = faust_wrap_add(l52, 1)) {
			iRec47[l52] = 0;
		}
		for (int l53 = 0; l53 < 2; l53 = faust_wrap_add(l53, 1)) {
			iRec48[l53] = 0;
		}
		for (int l54 = 0; l54 < 2; l54 = faust_wrap_add(l54, 1)) {
			fRec46[l54] = 0.0f;
		}
		for (int l55 = 0; l55 < 2; l55 = faust_wrap_add(l55, 1)) {
			fRec49[l55] = 0.0f;
		}
		for (int l56 = 0; l56 < 2; l56 = faust_wrap_add(l56, 1)) {
			fRec45[l56] = 0.0f;
		}
		for (int l57 = 0; l57 < 2; l57 = faust_wrap_add(l57, 1)) {
			fRec50[l57] = 0.0f;
		}
		for (int l58 = 0; l58 < 2; l58 = faust_wrap_add(l58, 1)) {
			fRec51[l58] = 0.0f;
		}
		for (int l59 = 0; l59 < 2; l59 = faust_wrap_add(l59, 1)) {
			fRec52[l59] = 0.0f;
		}
		for (int l60 = 0; l60 < 2; l60 = faust_wrap_add(l60, 1)) {
			fRec53[l60] = 0.0f;
		}
		for (int l61 = 0; l61 < 2; l61 = faust_wrap_add(l61, 1)) {
			fVec16[l61] = 0.0f;
		}
		for (int l62 = 0; l62 < 2; l62 = faust_wrap_add(l62, 1)) {
			fRec54[l62] = 0.0f;
		}
		for (int l63 = 0; l63 < 2; l63 = faust_wrap_add(l63, 1)) {
			fVec17[l63] = 0.0f;
		}
		for (int l64 = 0; l64 < 2; l64 = faust_wrap_add(l64, 1)) {
			fRec44[l64] = 0.0f;
		}
		for (int l65 = 0; l65 < 2; l65 = faust_wrap_add(l65, 1)) {
			fRec43[l65] = 0.0f;
		}
		for (int l66 = 0; l66 < 8192; l66 = faust_wrap_add(l66, 1)) {
			fVec18[l66] = 0.0f;
		}
		for (int l67 = 0; l67 < 2; l67 = faust_wrap_add(l67, 1)) {
			fRec15[l67] = 0.0f;
		}
		for (int l68 = 0; l68 < 8192; l68 = faust_wrap_add(l68, 1)) {
			fRec16[l68] = 0.0f;
		}
		for (int l69 = 0; l69 < 2; l69 = faust_wrap_add(l69, 1)) {
			fVec19[l69] = 0.0f;
		}
		for (int l70 = 0; l70 < 2; l70 = faust_wrap_add(l70, 1)) {
			fRec4[l70] = 0.0f;
		}
		for (int l71 = 0; l71 < 2; l71 = faust_wrap_add(l71, 1)) {
			fRec3[l71] = 0.0f;
		}
		for (int l72 = 0; l72 < 3; l72 = faust_wrap_add(l72, 1)) {
			fRec2[l72] = 0.0f;
		}
		for (int l73 = 0; l73 < 2; l73 = faust_wrap_add(l73, 1)) {
			fVec20[l73] = 0.0f;
		}
		for (int l74 = 0; l74 < 2; l74 = faust_wrap_add(l74, 1)) {
			fRec1[l74] = 0.0f;
		}
		for (int l75 = 0; l75 < 2; l75 = faust_wrap_add(l75, 1)) {
			fRec0[l75] = 0.0f;
		}
	}
	
	virtual void init(int sample_rate) {
		classInit(sample_rate);
		instanceInit(sample_rate);
	}
	
	virtual void instanceInit(int sample_rate) {
		instanceConstants(sample_rate);
		instanceResetUserInterface();
		instanceClear();
	}
	
	virtual DSP_Bass* clone() {
		return new DSP_Bass(*this);
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("OneTrick CHONK Bass");
		ui_interface->addNumEntry("Aftertouch", &fEntry1, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.001f));
		ui_interface->addButton("Articulation_Finger", &fButton9);
		ui_interface->addButton("Articulation_Frets", &fButton4);
		ui_interface->addButton("Articulation_Glide", &fButton3);
		ui_interface->addButton("Articulation_Legato", &fButton2);
		ui_interface->addButton("Articulation_Mute", &fButton5);
		ui_interface->addButton("Articulation_Pick", &fButton8);
		ui_interface->addButton("Articulation_Ring", &fButton0);
		ui_interface->addButton("Articulation_Slap", &fButton7);
		ui_interface->addNumEntry("Articulation_SlideDown", &fEntry4, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addNumEntry("Articulation_SlideUp", &fEntry3, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addNumEntry("ModWheel", &fEntry0, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.001f));
		ui_interface->addNumEntry("Pick_Up", &fEntry8, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("PitchWheel", &fEntry7, FAUSTFLOAT(0.0f), FAUSTFLOAT(-1.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.001f));
		ui_interface->addNumEntry("Transpose", &fEntry6, FAUSTFLOAT(0.0f), FAUSTFLOAT(-48.0f), FAUSTFLOAT(48.0f), FAUSTFLOAT(0.001f));
		ui_interface->addNumEntry("Trigger", &fEntry5, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addButton("WakeUp", &fButton1);
		ui_interface->declare(&fHslider8, "210", "");
		ui_interface->declare(&fHslider8, "export", "Pickup Position");
		ui_interface->declare(&fHslider8, "group", "Bass");
		ui_interface->declare(&fHslider8, "unit", "%");
		ui_interface->addHorizontalSlider("Pickup_Position", &fHslider8, FAUSTFLOAT(5e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1e+02f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fHslider3, "220", "");
		ui_interface->declare(&fHslider3, "export", "Sustain");
		ui_interface->declare(&fHslider3, "group", "Bass");
		ui_interface->declare(&fHslider3, "unit", "%");
		ui_interface->addHorizontalSlider("Sustain", &fHslider3, FAUSTFLOAT(1e+02f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1e+02f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fHslider1, "230", "");
		ui_interface->declare(&fHslider1, "export", "String Brightness");
		ui_interface->declare(&fHslider1, "group", "Bass");
		ui_interface->declare(&fHslider1, "unit", "%");
		ui_interface->addHorizontalSlider("Brightness", &fHslider1, FAUSTFLOAT(5e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1e+02f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fHslider10, "240", "");
		ui_interface->declare(&fHslider10, "export", "Strike Hardness");
		ui_interface->declare(&fHslider10, "group", "Bass");
		ui_interface->declare(&fHslider10, "unit", "%");
		ui_interface->addHorizontalSlider("StrikeHardness", &fHslider10, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1e+02f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fHslider11, "250", "");
		ui_interface->declare(&fHslider11, "export", "Thump");
		ui_interface->declare(&fHslider11, "group", "Bass");
		ui_interface->declare(&fHslider11, "unit", "%");
		ui_interface->addHorizontalSlider("Bass_Thump", &fHslider11, FAUSTFLOAT(5e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1e+02f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fHslider0, "260", "");
		ui_interface->declare(&fHslider0, "export", "Tone Knob");
		ui_interface->declare(&fHslider0, "group", "Bass");
		ui_interface->declare(&fHslider0, "unit", "%");
		ui_interface->addHorizontalSlider("Tone_Knob", &fHslider0, FAUSTFLOAT(5e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1e+02f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fHslider2, "270", "");
		ui_interface->declare(&fHslider2, "export", "Ring");
		ui_interface->declare(&fHslider2, "group", "Bass");
		ui_interface->declare(&fHslider2, "unit", "%");
		ui_interface->addHorizontalSlider("Ring", &fHslider2, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1e+02f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fHslider5, "280", "");
		ui_interface->declare(&fHslider5, "export", "Glide Time");
		ui_interface->declare(&fHslider5, "group", "Bass");
		ui_interface->declare(&fHslider5, "unit", "ms");
		ui_interface->addHorizontalSlider("Glide_Time", &fHslider5, FAUSTFLOAT(35.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(5e+02f), FAUSTFLOAT(0.1f));
		ui_interface->declare(&fHslider9, "290", "");
		ui_interface->declare(&fHslider9, "export", "Pick Depth");
		ui_interface->declare(&fHslider9, "group", "Bass");
		ui_interface->declare(&fHslider9, "unit", "%");
		ui_interface->addHorizontalSlider("Pick_Depth", &fHslider9, FAUSTFLOAT(1e+02f), FAUSTFLOAT(0.0f), FAUSTFLOAT(2e+02f), FAUSTFLOAT(1.0f));
		ui_interface->declare(&fHslider7, "310", "");
		ui_interface->declare(&fHslider7, "export", "Fine Tune");
		ui_interface->declare(&fHslider7, "group", "MIDI");
		ui_interface->declare(&fHslider7, "unit", "c");
		ui_interface->addHorizontalSlider("Fine_Tune", &fHslider7, FAUSTFLOAT(0.0f), FAUSTFLOAT(-1e+02f), FAUSTFLOAT(1e+02f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fHslider4, "320", "");
		ui_interface->declare(&fHslider4, "export", "Aftertouch Range");
		ui_interface->declare(&fHslider4, "group", "MIDI");
		ui_interface->declare(&fHslider4, "unit", "st");
		ui_interface->addHorizontalSlider("Midi_Afterouch_range", &fHslider4, FAUSTFLOAT(1.0f), FAUSTFLOAT(-2.0f), FAUSTFLOAT(2.0f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fHslider6, "330", "");
		ui_interface->declare(&fHslider6, "export", "PitchWheel Range");
		ui_interface->declare(&fHslider6, "group", "MIDI");
		ui_interface->declare(&fHslider6, "unit", "st");
		ui_interface->addHorizontalSlider("Midi_PitchWheel_range", &fHslider6, FAUSTFLOAT(2.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(12.0f), FAUSTFLOAT(0.01f));
		ui_interface->addNumEntry("gain", &fEntry9, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addButton("gate", &fButton6);
		ui_interface->addNumEntry("key", &fEntry2, FAUSTFLOAT(36.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(128.0f), FAUSTFLOAT(1.0f));
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {
		FAUSTFLOAT* output0 = outputs[0];
		float fSlow0 = 1.0f / std::tan(fConst3 * std::pow(2.0f, 0.083333336f * (0.36706725f * static_cast<float>(fHslider0) + -6.630492f)));
		float fSlow1 = 1.0f / (fSlow0 + 1.0f);
		float fSlow2 = static_cast<float>(fHslider1);
		float fSlow3 = std::min<float>(0.0f, std::max<float>(-1.0f, -(0.1f - 0.0006f * fSlow2)));
		float fSlow4 = std::tan(fConst15 * std::pow(3.25f, -fSlow3));
		float fSlow5 = fConst28 * fSlow4;
		float fSlow6 = fConst14 + fSlow5;
		float fSlow7 = 0.01f * static_cast<float>(fHslider2);
		float fSlow8 = static_cast<float>(fButton0);
		float fSlow9 = static_cast<float>(fButton1);
		float fSlow10 = 0.01f * static_cast<float>(fHslider3);
		float fSlow11 = 0.02f * (fSlow10 + static_cast<float>(fEntry0) * (-1.0f - fSlow10)) + 0.98f;
		float fSlow12 = static_cast<float>(fEntry1) * static_cast<float>(fHslider4);
		float fSlow13 = std::max<float>(0.0f, static_cast<float>(fEntry2) + -24.0f);
		float fSlow14 = 0.001f * static_cast<float>(fHslider5);
		float fSlow15 = static_cast<float>(fButton2);
		float fSlow16 = static_cast<float>(fButton3);
		float fSlow17 = static_cast<float>(fEntry3) - static_cast<float>(fEntry4);
		float fSlow18 = 48.0f * fSlow17 * static_cast<float>(((48.0f * fSlow17) != 0.0f) > 0);
		float fSlow19 = static_cast<float>(fEntry5);
		float fSlow20 = static_cast<float>(fButton4);
		float fSlow21 = static_cast<float>(fEntry6) + static_cast<float>(fHslider6) * static_cast<float>(fEntry7);
		float fSlow22 = 0.01f * static_cast<float>(fHslider7);
		float fSlow23 = static_cast<float>(fButton5);
		float fSlow24 = static_cast<float>(fButton6);
		float fSlow25 = fSlow5 - fConst14;
		float fSlow26 = std::tan(fConst15 * std::pow(3.25f, 1.0f - fSlow3));
		float fSlow27 = fConst28 * fSlow26;
		float fSlow28 = fConst16 * ((fConst14 + fSlow27) / fSlow4);
		float fSlow29 = fSlow27 - fConst14;
		float fSlow30 = fConst16 / fSlow4;
		float fSlow31 = fConst11 / fSlow26;
		float fSlow32 = 0.08f * static_cast<float>(fHslider8);
		float fSlow33 = 3.0f * (fSlow32 + 12.0f);
		float fSlow34 = 0.023255814f * (fSlow32 + (12.0f - fSlow33));
		float fSlow35 = 0.003f * fSlow2;
		float fSlow36 = fSlow35 + -9.05f;
		float fSlow37 = fSlow35 + -6.549995f;
		float fSlow38 = fSlow35 + -8.05f;
		float fSlow39 = fSlow35 + -7.05f;
		float fSlow40 = fSlow35 + -6.05f;
		float fSlow41 = fSlow35 + -5.05f;
		float fSlow42 = static_cast<float>(fEntry8);
		float fSlow43 = 1.0f - 2.0f * fSlow42;
		float fSlow44 = static_cast<float>(fEntry9);
		int iSlow45 = (fSlow13 >= 24.0f) * (fSlow13 <= 67.0f);
		float fSlow46 = static_cast<float>(fHslider9) * fSlow42;
		float fSlow47 = 0.33333334f * (1.0f - 0.0018f * fSlow46);
		float fSlow48 = 1.0f / std::tan(fConst45 * (26.0f * fSlow46 + 3.75e+03f));
		float fSlow49 = 1.0f / (fSlow48 + 1.0f);
		float fSlow50 = 1.0f - fSlow48;
		float fSlow51 = 1.0f / std::tan(fConst45 * (2.4f * fSlow46 + 3e+02f));
		float fSlow52 = 1.0f / (fSlow51 + 1.0f);
		float fSlow53 = 1.0f - fSlow51;
		float fSlow54 = 0.01f * static_cast<float>(fHslider10);
		float fSlow55 = static_cast<float>(fButton7);
		float fSlow56 = static_cast<float>(fButton8);
		float fSlow57 = static_cast<float>(fButton9);
		float fSlow58 = fConst49 * std::max<float>(0.1f, 1.0f - 0.0038f * fSlow46);
		float fSlow59 = 0.03f * static_cast<float>(fHslider11);
		float fSlow60 = 1.5f - fSlow59;
		float fSlow61 = 1.0f - fSlow0;
		for (int i0 = 0; i0 < count; i0 = faust_wrap_add(i0, 1)) {
			iVec0[0] = 1;
			fVec1[0] = fSlow9;
			float fTemp0 = static_cast<float>(fSlow9 <= fVec1[1]);
			float fTemp1 = 1.0f - fConst29 * fTemp0;
			fRec18[0] = fSlow8 * fTemp1 + fConst29 * fTemp0 * fRec18[1];
			fRec20[0] = fSlow12 * fTemp1 + fConst29 * fTemp0 * fRec20[1];
			fRec22[0] = fSlow15 * fTemp1 + fConst29 * fTemp0 * fRec22[1];
			fRec23[0] = fSlow16 * fTemp1 + fConst29 * fTemp0 * fRec23[1];
			float fTemp2 = fSlow14 * std::max<float>(fRec22[0], fRec23[0]);
			int iTemp3 = std::fabs(fTemp2) < 1.1920929e-07f;
			float fTemp4 = ((iTemp3) ? 0.0f : std::exp(-(fConst31 / ((iTemp3) ? 1.0f : fTemp2))));
			fRec21[0] = fSlow13 * (1.0f - fTemp4) + fTemp4 * fRec21[1];
			fVec2[0] = fSlow19;
			float fTemp5 = ((fSlow19 > fVec2[1]) ? fSlow19 : 0.0f);
			fVec3[0] = fTemp5;
			int iTemp6 = fTemp5 > fVec3[1];
			fRec24[0] = fSlow18 + fRec24[1] * static_cast<float>(iTemp6 == 0);
			float fTemp7 = fRec21[0] + fConst31 * fRec24[0];
			fRec25[0] = fSlow20 * fTemp1 + fConst29 * fTemp0 * fRec25[1];
			float fTemp8 = std::floor(fTemp7);
			float fTemp9 = fTemp7 - fTemp8;
			float fTemp10 = 1.0f - fConst32 * fTemp0;
			fRec26[0] = fSlow21 * fTemp10 + fConst32 * fTemp0 * fRec26[1];
			fRec19[0] = (1.0f - fConst30 * fTemp0) * std::min<float>(67.0f, std::max<float>(24.0f, fRec20[0] + fTemp7 + fRec25[0] * (fTemp8 + ((fTemp9 < 0.4125f) ? 0.0f : ((fTemp9 < 0.5875f) ? 5.714286f * (-0.4125f - fTemp8 + fTemp7) : 1.0f)) - fTemp7) + fRec26[0])) + fConst30 * fTemp0 * fRec19[1];
			fRec27[0] = fSlow22 * fTemp10 + fConst32 * fTemp0 * fRec27[1];
			float fTemp11 = std::pow(2.0f, 0.083333336f * (-69.0f + fRec19[0])) * std::pow(2.0f, 0.083333336f * fRec27[0]);
			float fTemp12 = -32.703197f + 4.4e+02f * fTemp11;
			fRec28[0] = fSlow23 * fTemp1 + fConst29 * fTemp0 * fRec28[1];
			float fTemp13 = fSlow11 * (0.9945f + 1.5029548e-05f * fTemp12) * (1.0f + fRec28[0] * (-0.25f + 0.00055664993f * fTemp12));
			float fTemp14 = std::exp(-(std::exp(-0.36651292f + std::max<float>(fSlow7, fRec18[0]) * (0.36651292f + std::log(std::max<float>(1e-06f, -(std::log(fTemp13))))))));
			fRec29[0] = fSlow24 * fTemp1 + fConst29 * fTemp0 * fRec29[1];
			float fTemp15 = fTemp14 + fRec29[0] * (fTemp13 - fTemp14);
			fVec4[0] = fTemp15;
			float fTemp16 = ((fTemp15 != fVec4[1]) ? fConst33 : -1.0f + fRec30[1]);
			fRec30[0] = fTemp16;
			fRec17[0] = ((fTemp16 > 0.0f) ? fRec17[1] + (fTemp15 - fRec17[1]) / fTemp16 : fTemp15);
			float fTemp17 = fRec7[1] * fRec17[0];
			fVec5[0] = fTemp17;
			fRec5[0] = -(fConst24 * (fSlow6 * fTemp17 + fSlow25 * fVec5[1] + fConst25 * fRec5[1]));
			fVec6[0] = fSlow30 * fRec5[0];
			fRec6[0] = -(fConst18 * (fConst19 * fRec6[1] - (fSlow28 * fRec5[0] + fSlow29 * fVec6[1])));
			fRec7[0] = fRec10[1];
			fRec8[IOTA0 & 8191] = fSlow31 * fRec6[0];
			float fTemp18 = fRec17[0] * fRec15[1];
			fVec7[0] = fTemp18;
			fRec9[0] = -(fConst24 * (fSlow6 * fTemp18 + fSlow25 * fVec7[1] + fConst25 * fRec9[1]));
			float fTemp19 = fSlow33 + fSlow34 * (-24.0f + fRec19[0]);
			float fTemp20 = fConst34 * fTemp19;
			float fTemp21 = -1.499995f + fTemp20;
			float fTemp22 = std::floor(fTemp21);
			float fTemp23 = -4.0f - fTemp22 + fTemp20;
			float fTemp24 = -3.0f - fTemp22 + fTemp20;
			float fTemp25 = -2.0f - fTemp22 + fTemp20;
			int iTemp26 = static_cast<int>(fTemp21);
			int iTemp27 = faust_wrap_add(1, static_cast<int>(std::min<float>(fConst35, static_cast<float>(std::max<int>(0, iTemp26)))));
			float fTemp28 = -1.0f - fTemp22 + fTemp20;
			float fTemp29 = fTemp20 - fTemp22;
			int iTemp30 = faust_wrap_add(1, static_cast<int>(std::min<float>(fConst35, static_cast<float>(std::max<int>(0, faust_wrap_add(1, iTemp26))))));
			float fTemp31 = fTemp29 * fTemp28;
			int iTemp32 = faust_wrap_add(1, static_cast<int>(std::min<float>(fConst35, static_cast<float>(std::max<int>(0, faust_wrap_add(2, iTemp26))))));
			float fTemp33 = fTemp31 * fTemp25;
			int iTemp34 = faust_wrap_add(1, static_cast<int>(std::min<float>(fConst35, static_cast<float>(std::max<int>(0, faust_wrap_add(3, iTemp26))))));
			float fTemp35 = fTemp33 * fTemp24;
			int iTemp36 = faust_wrap_add(1, static_cast<int>(std::min<float>(fConst35, static_cast<float>(std::max<int>(0, faust_wrap_add(4, iTemp26))))));
			fRec10[0] = fTemp23 * (fTemp24 * (fTemp25 * (0.041666668f * fRec16[(faust_wrap_sub(IOTA0, iTemp27)) & 8191] * fTemp28 - 0.16666667f * fTemp29 * fRec16[(faust_wrap_sub(IOTA0, iTemp30)) & 8191]) + 0.25f * fTemp31 * fRec16[(faust_wrap_sub(IOTA0, iTemp32)) & 8191]) - 0.16666667f * fTemp33 * fRec16[(faust_wrap_sub(IOTA0, iTemp34)) & 8191]) + 0.041666668f * fTemp35 * fRec16[(faust_wrap_sub(IOTA0, iTemp36)) & 8191];
			float fTemp37 = fConst0 * (0.0011363636f / fTemp11 - 2.9411765e-05f * fTemp19);
			float fTemp38 = fSlow37 + fTemp37;
			float fTemp39 = std::floor(fTemp38);
			float fTemp40 = fSlow36 - fTemp39 + fTemp37;
			float fTemp41 = fSlow38 - fTemp39 + fTemp37;
			float fTemp42 = fSlow39 - fTemp39 + fTemp37;
			int iTemp43 = static_cast<int>(fTemp38);
			int iTemp44 = faust_wrap_add(1, static_cast<int>(std::min<float>(fConst35, static_cast<float>(std::max<int>(0, iTemp43)))));
			float fTemp45 = fSlow40 - fTemp39 + fTemp37;
			float fTemp46 = fSlow41 - fTemp39 + fTemp37;
			int iTemp47 = faust_wrap_add(1, static_cast<int>(std::min<float>(fConst35, static_cast<float>(std::max<int>(0, faust_wrap_add(1, iTemp43))))));
			float fTemp48 = fTemp46 * fTemp45;
			int iTemp49 = faust_wrap_add(1, static_cast<int>(std::min<float>(fConst35, static_cast<float>(std::max<int>(0, faust_wrap_add(2, iTemp43))))));
			float fTemp50 = fTemp48 * fTemp42;
			int iTemp51 = faust_wrap_add(1, static_cast<int>(std::min<float>(fConst35, static_cast<float>(std::max<int>(0, faust_wrap_add(3, iTemp43))))));
			float fTemp52 = fTemp50 * fTemp41;
			int iTemp53 = faust_wrap_add(1, static_cast<int>(std::min<float>(fConst35, static_cast<float>(std::max<int>(0, faust_wrap_add(4, iTemp43))))));
			fRec11[0] = fTemp40 * (fTemp41 * (fTemp42 * (0.041666668f * fRec14[(faust_wrap_sub(IOTA0, iTemp44)) & 8191] * fTemp45 - 0.16666667f * fTemp46 * fRec14[(faust_wrap_sub(IOTA0, iTemp47)) & 8191]) + 0.25f * fTemp48 * fRec14[(faust_wrap_sub(IOTA0, iTemp49)) & 8191]) - 0.16666667f * fTemp50 * fRec14[(faust_wrap_sub(IOTA0, iTemp51)) & 8191]) + 0.041666668f * fTemp52 * fRec14[(faust_wrap_sub(IOTA0, iTemp53)) & 8191];
			fVec8[0] = fSlow30 * fRec9[0];
			fRec12[0] = -(fConst18 * (fConst19 * fRec12[1] - (fSlow28 * fRec9[0] + fSlow29 * fVec8[1])));
			fVec9[0] = fTemp23 * (fTemp24 * (fTemp25 * (0.041666668f * fTemp28 * fRec8[(faust_wrap_sub(IOTA0, iTemp27)) & 8191] - 0.16666667f * fTemp29 * fRec8[(faust_wrap_sub(IOTA0, iTemp30)) & 8191]) + 0.25f * fTemp31 * fRec8[(faust_wrap_sub(IOTA0, iTemp32)) & 8191]) - 0.16666667f * fTemp33 * fRec8[(faust_wrap_sub(IOTA0, iTemp34)) & 8191]) + 0.041666668f * fTemp35 * fRec8[(faust_wrap_sub(IOTA0, iTemp36)) & 8191];
			float fTemp54 = fRec11[1] + fVec9[1];
			float fTemp55 = std::fabs(fTemp54);
			fRec13[0] = std::max<float>(fTemp55, fConst32 * fRec13[1] + fConst36 * fTemp55);
			fRec14[IOTA0 & 8191] = fSlow31 * fRec12[0];
			fRec31[0] = ((iTemp6) ? fTemp5 : fRec31[1]);
			float fTemp56 = std::max<float>(fSlow44, fRec31[0]);
			float fTemp57 = 0.8f * fTemp56;
			float fTemp58 = std::min<float>(1.0f, std::max<float>(0.0f, 5.0f * (0.3f - 3.0f * fRec13[0] + fTemp57)));
			int iTemp59 = ((iSlow45) ? iTemp6 : 0);
			iVec11[0] = iTemp59;
			int iTemp60 = iTemp59 > iVec11[1];
			iVec12[0] = iTemp60;
			int iTemp61 = (faust_wrap_sub(1, iVec0[1])) | iTemp60;
			float fTemp62 = ((iTemp61) ? 0.0f : fRec33[1] + fConst37 * fTemp11);
			fRec33[0] = fTemp62 - std::floor(fTemp62);
			float fTemp63 = ((iTemp61) ? 0.0f : fRec34[1] + fConst38 * fTemp11);
			fRec34[0] = fTemp63 - std::floor(fTemp63);
			float fTemp64 = ((iTemp61) ? 0.0f : fRec35[1] + fConst39 * fTemp11);
			fRec35[0] = fTemp64 - std::floor(fTemp64);
			float fTemp65 = ((iTemp61) ? 0.0f : fRec36[1] + fConst40 * fTemp11);
			fRec36[0] = fTemp65 - std::floor(fTemp65);
			int iTemp66 = iTemp60 > iVec12[1];
			iVec13[0] = iTemp66;
			iRec37[0] = faust_wrap_add(iTemp66, faust_wrap_mul(faust_wrap_add(iRec37[1], iRec37[1] > 0), iTemp60 <= iVec12[1]));
			float fTemp67 = static_cast<float>(iRec37[0]);
			float fTemp68 = std::max<float>(0.0f, std::min<float>(fTemp67, 1.0f + fConst43 * (1.0f - fTemp67)));
			float fTemp69 = std::max<float>(std::min<float>(0.0005681818f / fTemp11, 0.002f), 0.00015f);
			float fTemp70 = fConst0 * fTemp69;
			fRec39[0] = ((iTemp66 > 0) ? fTemp70 : std::max<float>(0.0f, -1.0f + fRec39[1]));
			iRec40[0] = (iTemp66 > iVec13[1]) + faust_wrap_mul(faust_wrap_add(iRec40[1], iRec40[1] > 0), iTemp66 <= iVec13[1]);
			float fTemp71 = static_cast<float>(iRec40[0]);
			float fTemp72 = std::max<float>(1.0f, fTemp70);
			float fTemp73 = std::max<float>(0.0f, std::min<float>(fTemp71 / fTemp72, 1.0f + (fTemp72 - fTemp71) / std::max<float>(1.0f, fConst44 / fTemp11)));
			fVec14[0] = fTemp73;
			fRec41[0] = ((iTemp66) ? fVec14[1] : fRec41[1]);
			float fTemp74 = fSlow47 * (((fConst31 * (fRec39[0] / fTemp69)) > 0.0f) ? fTemp73 + fConst31 * (fRec39[0] * (fRec41[0] - fTemp73) / fTemp69) : fTemp73) * (0.2f + fTemp57);
			fVec15[0] = fTemp74;
			fRec38[0] = ((iTemp60) ? fVec15[1] : fRec38[1]);
			fRec42[0] = ((iTemp60) ? fVec15[1] - fVec15[2] : fRec42[1]);
			float fTemp75 = 1.0f - fTemp68;
			float fTemp76 = fTemp68 * (fRec38[0] + fConst41 * fRec42[0] * fTemp75);
			float fTemp77 = (ftbl0DSP_BassSIG0[static_cast<int>(65536.0f * fRec33[0])] + 0.5f * ftbl0DSP_BassSIG0[static_cast<int>(65536.0f * fRec34[0])] + 0.33333334f * ftbl0DSP_BassSIG0[static_cast<int>(65536.0f * fRec35[0])] + 0.25f * ftbl0DSP_BassSIG0[static_cast<int>(65536.0f * fRec36[0])]) * (fTemp76 + fTemp75 * (fTemp74 - fTemp76));
			iRec47[0] = faust_wrap_add(1, iRec47[1]);
			iRec48[0] = faust_wrap_add(12345, faust_wrap_mul(1103515245, iRec48[1]));
			fRec46[0] = (((iRec47[1] % iConst47) == 0) ? 4.656613e-10f * static_cast<float>(iRec48[0]) : fRec46[1]);
			float fTemp78 = ((fRec46[0] != fRec46[1]) ? fConst46 : -1.0f + fRec49[1]);
			fRec49[0] = fTemp78;
			fRec45[0] = ((fTemp78 > 0.0f) ? fRec45[1] + (fRec46[0] - fRec45[1]) / fTemp78 : fRec46[0]);
			fRec50[0] = fSlow55 * fTemp1 + fConst29 * fTemp0 * fRec50[1];
			float fTemp79 = std::max<float>(std::pow(std::max<float>(0.0f, 6.6666665f * (-0.85f + fTemp56)), 1.5f), fRec50[0]);
			fRec51[0] = fSlow56 * fTemp1 + fConst29 * fTemp0 * fRec51[1];
			fRec52[0] = fSlow57 * fTemp1 + fConst29 * fTemp0 * fRec52[1];
			float fTemp80 = fSlow54 + std::max<float>(fTemp79, std::max<float>(fRec51[0], fRec52[0])) * (std::max<float>(0.5f * fRec51[0], fTemp79) - fSlow54);
			float fTemp81 = std::pow(fTemp56, 1.5f);
			fRec53[0] = ((iTemp60 > 0) ? fConst41 : std::max<float>(0.0f, -1.0f + fRec53[1]));
			float fTemp82 = std::max<float>(0.0f, std::min<float>(fConst43 * fTemp67, 1.0f + (fConst42 - fTemp67) / std::max<float>(1.0f, fSlow58 * (2.0f + 6.0f * fTemp80 * fTemp81))));
			fVec16[0] = fTemp82;
			fRec54[0] = ((iTemp60) ? fVec16[1] : fRec54[1]);
			float fTemp83 = fRec45[0] * fTemp80 * fTemp81 * (((fConst48 * fRec53[0]) > 0.0f) ? fTemp82 + fConst48 * fRec53[0] * (fRec54[0] - fTemp82) : fTemp82);
			fVec17[0] = fTemp83;
			fRec44[0] = -(fSlow52 * (fSlow53 * fRec44[1] - fSlow51 * (fTemp83 - fVec17[1])));
			fRec43[0] = -(fSlow49 * (fSlow50 * fRec43[1] - (fRec44[1] + fRec44[0])));
			float fTemp84 = 1.0f - fRec22[0];
			float fTemp85 = fRec43[0] * fTemp84;
			float fTemp86 = 0.4379562f * fTemp77 + fTemp85;
			float fTemp87 = fSlow43 * fTemp58 * fTemp86;
			float fTemp88 = fVec9[1] + fTemp87;
			fVec18[IOTA0 & 8191] = fTemp88;
			fRec15[0] = fTemp40 * (fTemp41 * (fTemp42 * (0.041666668f * fTemp45 * fVec18[(faust_wrap_sub(IOTA0, iTemp44)) & 8191] - 0.16666667f * fTemp46 * fVec18[(faust_wrap_sub(IOTA0, iTemp47)) & 8191]) + 0.25f * fTemp48 * fVec18[(faust_wrap_sub(IOTA0, iTemp49)) & 8191]) - 0.16666667f * fTemp50 * fVec18[(faust_wrap_sub(IOTA0, iTemp51)) & 8191]) + 0.041666668f * fTemp52 * fVec18[(faust_wrap_sub(IOTA0, iTemp53)) & 8191];
			fRec16[IOTA0 & 8191] = fRec11[1] + fTemp87;
			float fTemp89 = fTemp54 + fSlow43 * fTemp58 * fTemp84 * (fTemp85 + fTemp86 + 0.4379562f * fTemp77 * (fSlow59 + fSlow60 * fRec28[0]) * (1.0f - DSP_Bass_faustpower2_f(1.0f - fTemp56)));
			fVec19[0] = fTemp89;
			fRec4[0] = -(fConst24 * (fConst25 * fRec4[1] - (fConst27 * fTemp89 + fConst50 * fVec19[1])));
			fRec3[0] = -(fConst18 * (fConst19 * fRec3[1] - fConst21 * (fConst23 * fRec4[0] + fConst51 * fRec4[1])));
			float fTemp90 = fConst10 * fRec2[1];
			fRec2[0] = fConst13 * fRec3[0] - fConst52 * (fConst53 * fRec2[2] + fTemp90);
			float fTemp91 = fTemp90 + fConst55 * fRec2[0] + fConst56 * fRec2[2];
			fVec20[0] = fTemp91;
			fRec1[0] = fSlow1 * (fConst9 * (fTemp91 + fVec20[1]) - fSlow61 * fRec1[1]);
			fRec0[0] = fConst2 * (fRec1[0] - fRec1[1] + fConst57 * fRec0[1]);
			output0[i0] = static_cast<FAUSTFLOAT>(fRec0[0]);
			iVec0[1] = iVec0[0];
			fVec1[1] = fVec1[0];
			fRec18[1] = fRec18[0];
			fRec20[1] = fRec20[0];
			fRec22[1] = fRec22[0];
			fRec23[1] = fRec23[0];
			fRec21[1] = fRec21[0];
			fVec2[1] = fVec2[0];
			fVec3[1] = fVec3[0];
			fRec24[1] = fRec24[0];
			fRec25[1] = fRec25[0];
			fRec26[1] = fRec26[0];
			fRec19[1] = fRec19[0];
			fRec27[1] = fRec27[0];
			fRec28[1] = fRec28[0];
			fRec29[1] = fRec29[0];
			fVec4[1] = fVec4[0];
			fRec30[1] = fRec30[0];
			fRec17[1] = fRec17[0];
			fVec5[1] = fVec5[0];
			fRec5[1] = fRec5[0];
			fVec6[1] = fVec6[0];
			fRec6[1] = fRec6[0];
			fRec7[1] = fRec7[0];
			IOTA0 = faust_wrap_add(IOTA0, 1);
			fVec7[1] = fVec7[0];
			fRec9[1] = fRec9[0];
			fRec10[1] = fRec10[0];
			fRec11[1] = fRec11[0];
			fVec8[1] = fVec8[0];
			fRec12[1] = fRec12[0];
			fVec9[1] = fVec9[0];
			fRec13[1] = fRec13[0];
			fRec31[1] = fRec31[0];
			iVec11[1] = iVec11[0];
			iVec12[1] = iVec12[0];
			fRec33[1] = fRec33[0];
			fRec34[1] = fRec34[0];
			fRec35[1] = fRec35[0];
			fRec36[1] = fRec36[0];
			iVec13[1] = iVec13[0];
			iRec37[1] = iRec37[0];
			fRec39[1] = fRec39[0];
			iRec40[1] = iRec40[0];
			fVec14[1] = fVec14[0];
			fRec41[1] = fRec41[0];
			fVec15[2] = fVec15[1];
			fVec15[1] = fVec15[0];
			fRec38[1] = fRec38[0];
			fRec42[1] = fRec42[0];
			iRec47[1] = iRec47[0];
			iRec48[1] = iRec48[0];
			fRec46[1] = fRec46[0];
			fRec49[1] = fRec49[0];
			fRec45[1] = fRec45[0];
			fRec50[1] = fRec50[0];
			fRec51[1] = fRec51[0];
			fRec52[1] = fRec52[0];
			fRec53[1] = fRec53[0];
			fVec16[1] = fVec16[0];
			fRec54[1] = fRec54[0];
			fVec17[1] = fVec17[0];
			fRec44[1] = fRec44[0];
			fRec43[1] = fRec43[0];
			fRec15[1] = fRec15[0];
			fVec19[1] = fVec19[0];
			fRec4[1] = fRec4[0];
			fRec3[1] = fRec3[0];
			fRec2[2] = fRec2[1];
			fRec2[1] = fRec2[0];
			fVec20[1] = fVec20[0];
			fRec1[1] = fRec1[0];
			fRec0[1] = fRec0[0];
		}
	}

};

#endif
