/* ------------------------------------------------------------
copyright: "Copyright (c) 2024 Punk Labs LLC"
license: "GPLv3 (or later)"
name: "OneTrick CHONK Output"
Code generated with Faust 2.88.0 (https://faust.grame.fr)
Compilation options: -lang cpp -fpga-mem-th 4 -ct 0 -cn DSP_Output -dtl 65536 -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __DSP_Output_H__
#define  __DSP_Output_H__

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
#define FAUSTCLASS DSP_Output
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

static float DSP_Output_faustpower2_f(float value) {
	return value * value;
}

class DSP_Output : public dsp {
	
 private:
	
	FAUSTFLOAT fHslider0;
	int fSampleRate;
	float fConst0;
	float fConst1;
	float fConst2;
	float fConst3;
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
	float fVec0[2];
	float fRec6[2];
	float fConst19;
	float fConst20;
	float fConst21;
	float fRec5[3];
	FAUSTFLOAT fHslider1;
	float fConst22;
	FAUSTFLOAT fButton0;
	float fVec1[2];
	float fRec7[2];
	float fRec9[2];
	float fRec8[3];
	float fConst23;
	FAUSTFLOAT fHslider2;
	float fRec10[2];
	float fConst24;
	float fRec4[3];
	float fConst25;
	FAUSTFLOAT fHslider3;
	float fRec11[2];
	float fConst26;
	float fRec3[3];
	float fConst27;
	FAUSTFLOAT fHslider4;
	float fRec12[2];
	float fConst28;
	float fRec2[3];
	float fVec2[2];
	float fRec1[2];
	float fConst29;
	float fConst30;
	float fConst31;
	float fRec0[3];
	float fRec14[2];
	float fRec13[3];
	FAUSTFLOAT fHslider5;
	float fRec15[2];
	FAUSTFLOAT fHslider6;
	float fRec16[2];
	FAUSTFLOAT fHslider7;
	float fRec17[2];
	
 public:
	DSP_Output() {
	}
	
	DSP_Output(const DSP_Output&) = default;
	
	virtual ~DSP_Output() = default;
	
	DSP_Output& operator=(const DSP_Output&) = default;
	
	void metadata(Meta* m) { 
		m->declare("analyzers.lib/name", "Faust Analyzer Library");
		m->declare("analyzers.lib/version", "1.4.0");
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "1.23.0");
		m->declare("compile_options", "-lang cpp -fpga-mem-th 4 -ct 0 -cn DSP_Output -dtl 65536 -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0");
		m->declare("copyright", "Copyright (c) 2024 Punk Labs LLC");
		m->declare("filename", "output.dsp");
		m->declare("filters.lib/filterbank:author", "Julius O. Smith III");
		m->declare("filters.lib/filterbank:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/filterbank:license", "LicenseRef-STK-4.3");
		m->declare("filters.lib/fir:author", "Julius O. Smith III");
		m->declare("filters.lib/fir:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/fir:license", "LicenseRef-STK-4.3");
		m->declare("filters.lib/highpass:author", "Julius O. Smith III");
		m->declare("filters.lib/highpass:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/highpass:license", "LicenseRef-STK-4.3");
		m->declare("filters.lib/highshelf:author", "Julius O. Smith III");
		m->declare("filters.lib/highshelf:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/highshelf:license", "LicenseRef-STK-4.3");
		m->declare("filters.lib/iir:author", "Julius O. Smith III");
		m->declare("filters.lib/iir:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/iir:license", "LicenseRef-STK-4.3");
		m->declare("filters.lib/low_shelf:author", "Julius O. Smith III");
		m->declare("filters.lib/low_shelf:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/low_shelf:license", "LicenseRef-STK-4.3");
		m->declare("filters.lib/lowpass0_highpass1:author", "Julius O. Smith III");
		m->declare("filters.lib/lowpass0_highpass1:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/lowpass0_highpass1:license", "LicenseRef-STK-4.3");
		m->declare("filters.lib/lowpass:author", "Julius O. Smith III");
		m->declare("filters.lib/lowpass:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/lowpass:license", "LicenseRef-STK-4.3");
		m->declare("filters.lib/lowshelf:author", "Julius O. Smith III");
		m->declare("filters.lib/lowshelf:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/lowshelf:license", "LicenseRef-STK-4.3");
		m->declare("filters.lib/name", "Faust Filters Library");
		m->declare("filters.lib/peak_eq:author", "Julius O. Smith III");
		m->declare("filters.lib/peak_eq:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/peak_eq:license", "LicenseRef-STK-4.3");
		m->declare("filters.lib/peak_eq_cq:author", "Julius O. Smith III");
		m->declare("filters.lib/peak_eq_cq:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/peak_eq_cq:license", "LicenseRef-STK-4.3");
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
		m->declare("interpolators.lib/interpolate_linear:author", "Stéphane Letz");
		m->declare("interpolators.lib/interpolate_linear:licence", "MIT");
		m->declare("interpolators.lib/name", "Faust Interpolator Library");
		m->declare("interpolators.lib/version", "1.6.0");
		m->declare("license", "GPLv3 (or later)");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LicenseRef-LGPL-2.1-or-later-with-Faust-exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.9.0");
		m->declare("name", "OneTrick CHONK Output");
		m->declare("onetrick.lib/copyright", "Copyright (c) 2023 Punk Labs LLC");
		m->declare("onetrick.lib/license", "GPLv3 (or later)");
		m->declare("onetrick.lib/name", "OneTrick DSP Library");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "1.3.0");
		m->declare("signals.lib/name", "Faust Routing Library");
		m->declare("signals.lib/version", "1.7.0");
	}

	virtual int getNumInputs() {
		return 1;
	}
	virtual int getNumOutputs() {
		return 2;
	}
	
	static void classInit(int sample_rate) {
	}
	
	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		fConst0 = std::min<float>(1.92e+05f, std::max<float>(1.0f, static_cast<float>(fSampleRate)));
		fConst1 = 9424.778f / fConst0;
		fConst2 = std::tan(fConst1);
		fConst3 = 1.0f / fConst2;
		fConst4 = 1.0f / ((fConst3 + 1.0f) / fConst2 + 1.0f);
		fConst5 = 1.0f / (fConst3 + 1.0f);
		fConst6 = 1.0f - fConst3;
		fConst7 = std::tan(4712.389f / fConst0);
		fConst8 = 2.0f * (1.0f - 1.0f / DSP_Output_faustpower2_f(fConst7));
		fConst9 = 1570.7964f / fConst0;
		fConst10 = std::tan(fConst9);
		fConst11 = 2.0f * (1.0f - 1.0f / DSP_Output_faustpower2_f(fConst10));
		fConst12 = std::tan(785.3982f / fConst0);
		fConst13 = 2.0f * (1.0f - 1.0f / DSP_Output_faustpower2_f(fConst12));
		fConst14 = std::tan(314.15927f / fConst0);
		fConst15 = 1.0f / fConst14;
		fConst16 = 1.0f / ((fConst15 + 1.0f) / fConst14 + 1.0f);
		fConst17 = 1.0f / (fConst15 + 1.0f);
		fConst18 = 1.0f - fConst15;
		fConst19 = (fConst15 + -1.0f) / fConst14 + 1.0f;
		fConst20 = 1.0f / DSP_Output_faustpower2_f(fConst14);
		fConst21 = 2.0f * (1.0f - fConst20);
		fConst22 = std::exp(-(1e+02f / fConst0));
		fConst23 = 1.0f / fConst12;
		fConst24 = 1106.1946f / (fConst0 * std::sin(fConst9));
		fConst25 = 1.0f / fConst10;
		fConst26 = 2212.3892f / (fConst0 * std::sin(3141.5928f / fConst0));
		fConst27 = 1.0f / fConst7;
		fConst28 = 6637.1675f / (fConst0 * std::sin(fConst1));
		fConst29 = (fConst3 + -1.0f) / fConst2 + 1.0f;
		fConst30 = 1.0f / DSP_Output_faustpower2_f(fConst2);
		fConst31 = 2.0f * (1.0f - fConst30);
	}
	
	virtual void instanceResetUserInterface() {
		fHslider0 = static_cast<FAUSTFLOAT>(0.0f);
		fHslider1 = static_cast<FAUSTFLOAT>(0.0f);
		fButton0 = static_cast<FAUSTFLOAT>(0.0f);
		fHslider2 = static_cast<FAUSTFLOAT>(0.0f);
		fHslider3 = static_cast<FAUSTFLOAT>(0.0f);
		fHslider4 = static_cast<FAUSTFLOAT>(0.0f);
		fHslider5 = static_cast<FAUSTFLOAT>(0.0f);
		fHslider6 = static_cast<FAUSTFLOAT>(0.0f);
		fHslider7 = static_cast<FAUSTFLOAT>(0.0f);
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; l0 < 2; l0 = faust_wrap_add(l0, 1)) {
			fVec0[l0] = 0.0f;
		}
		for (int l1 = 0; l1 < 2; l1 = faust_wrap_add(l1, 1)) {
			fRec6[l1] = 0.0f;
		}
		for (int l2 = 0; l2 < 3; l2 = faust_wrap_add(l2, 1)) {
			fRec5[l2] = 0.0f;
		}
		for (int l3 = 0; l3 < 2; l3 = faust_wrap_add(l3, 1)) {
			fVec1[l3] = 0.0f;
		}
		for (int l4 = 0; l4 < 2; l4 = faust_wrap_add(l4, 1)) {
			fRec7[l4] = 0.0f;
		}
		for (int l5 = 0; l5 < 2; l5 = faust_wrap_add(l5, 1)) {
			fRec9[l5] = 0.0f;
		}
		for (int l6 = 0; l6 < 3; l6 = faust_wrap_add(l6, 1)) {
			fRec8[l6] = 0.0f;
		}
		for (int l7 = 0; l7 < 2; l7 = faust_wrap_add(l7, 1)) {
			fRec10[l7] = 0.0f;
		}
		for (int l8 = 0; l8 < 3; l8 = faust_wrap_add(l8, 1)) {
			fRec4[l8] = 0.0f;
		}
		for (int l9 = 0; l9 < 2; l9 = faust_wrap_add(l9, 1)) {
			fRec11[l9] = 0.0f;
		}
		for (int l10 = 0; l10 < 3; l10 = faust_wrap_add(l10, 1)) {
			fRec3[l10] = 0.0f;
		}
		for (int l11 = 0; l11 < 2; l11 = faust_wrap_add(l11, 1)) {
			fRec12[l11] = 0.0f;
		}
		for (int l12 = 0; l12 < 3; l12 = faust_wrap_add(l12, 1)) {
			fRec2[l12] = 0.0f;
		}
		for (int l13 = 0; l13 < 2; l13 = faust_wrap_add(l13, 1)) {
			fVec2[l13] = 0.0f;
		}
		for (int l14 = 0; l14 < 2; l14 = faust_wrap_add(l14, 1)) {
			fRec1[l14] = 0.0f;
		}
		for (int l15 = 0; l15 < 3; l15 = faust_wrap_add(l15, 1)) {
			fRec0[l15] = 0.0f;
		}
		for (int l16 = 0; l16 < 2; l16 = faust_wrap_add(l16, 1)) {
			fRec14[l16] = 0.0f;
		}
		for (int l17 = 0; l17 < 3; l17 = faust_wrap_add(l17, 1)) {
			fRec13[l17] = 0.0f;
		}
		for (int l18 = 0; l18 < 2; l18 = faust_wrap_add(l18, 1)) {
			fRec15[l18] = 0.0f;
		}
		for (int l19 = 0; l19 < 2; l19 = faust_wrap_add(l19, 1)) {
			fRec16[l19] = 0.0f;
		}
		for (int l20 = 0; l20 < 2; l20 = faust_wrap_add(l20, 1)) {
			fRec17[l20] = 0.0f;
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
	
	virtual DSP_Output* clone() {
		return new DSP_Output(*this);
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("OneTrick CHONK Output");
		ui_interface->addButton("WakeUp", &fButton0);
		ui_interface->declare(&fHslider6, "110", "");
		ui_interface->declare(&fHslider6, "export", "Gain");
		ui_interface->declare(&fHslider6, "group", "Mix");
		ui_interface->declare(&fHslider6, "unit", "dB");
		ui_interface->addHorizontalSlider("Mix_Gain", &fHslider6, FAUSTFLOAT(0.0f), FAUSTFLOAT(-1e+02f), FAUSTFLOAT(6.0f), FAUSTFLOAT(0.1f));
		ui_interface->declare(&fHslider7, "120", "");
		ui_interface->declare(&fHslider7, "export", "Pan");
		ui_interface->declare(&fHslider7, "group", "Mix");
		ui_interface->declare(&fHslider7, "unit", "%");
		ui_interface->addHorizontalSlider("Mix_Pan", &fHslider7, FAUSTFLOAT(0.0f), FAUSTFLOAT(-1e+02f), FAUSTFLOAT(1e+02f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fHslider0, "130", "");
		ui_interface->declare(&fHslider0, "export", "Saturation");
		ui_interface->declare(&fHslider0, "group", "Mix");
		ui_interface->declare(&fHslider0, "unit", "%");
		ui_interface->addHorizontalSlider("Mix_Saturation", &fHslider0, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1e+02f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fHslider1, "410", "");
		ui_interface->declare(&fHslider1, "export", "Band 1");
		ui_interface->declare(&fHslider1, "group", "EQ");
		ui_interface->declare(&fHslider1, "unit", "dB");
		ui_interface->addHorizontalSlider("EQ_Band_1", &fHslider1, FAUSTFLOAT(0.0f), FAUSTFLOAT(-12.0f), FAUSTFLOAT(12.0f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fHslider2, "420", "");
		ui_interface->declare(&fHslider2, "export", "Band 2");
		ui_interface->declare(&fHslider2, "group", "EQ");
		ui_interface->declare(&fHslider2, "unit", "dB");
		ui_interface->addHorizontalSlider("EQ_Band_2", &fHslider2, FAUSTFLOAT(0.0f), FAUSTFLOAT(-12.0f), FAUSTFLOAT(12.0f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fHslider3, "430", "");
		ui_interface->declare(&fHslider3, "export", "Band 3");
		ui_interface->declare(&fHslider3, "group", "EQ");
		ui_interface->declare(&fHslider3, "unit", "dB");
		ui_interface->addHorizontalSlider("EQ_Band_3", &fHslider3, FAUSTFLOAT(0.0f), FAUSTFLOAT(-12.0f), FAUSTFLOAT(12.0f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fHslider4, "440", "");
		ui_interface->declare(&fHslider4, "export", "Band 4");
		ui_interface->declare(&fHslider4, "group", "EQ");
		ui_interface->declare(&fHslider4, "unit", "dB");
		ui_interface->addHorizontalSlider("EQ_Band_4", &fHslider4, FAUSTFLOAT(0.0f), FAUSTFLOAT(-12.0f), FAUSTFLOAT(12.0f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fHslider5, "450", "");
		ui_interface->declare(&fHslider5, "export", "Band 5");
		ui_interface->declare(&fHslider5, "group", "EQ");
		ui_interface->declare(&fHslider5, "unit", "dB");
		ui_interface->addHorizontalSlider("EQ_Band_5", &fHslider5, FAUSTFLOAT(0.0f), FAUSTFLOAT(-12.0f), FAUSTFLOAT(12.0f), FAUSTFLOAT(0.01f));
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {
		FAUSTFLOAT* input0 = inputs[0];
		FAUSTFLOAT* output0 = outputs[0];
		FAUSTFLOAT* output1 = outputs[1];
		float fSlow0 = static_cast<float>(fHslider0);
		float fSlow1 = 1.0f - 0.00525f * fSlow0;
		float fSlow2 = static_cast<float>(fHslider1);
		float fSlow3 = static_cast<float>(fButton0);
		float fSlow4 = static_cast<float>(fHslider2);
		float fSlow5 = static_cast<float>(fHslider3);
		float fSlow6 = static_cast<float>(fHslider4);
		float fSlow7 = static_cast<float>(fHslider5);
		float fSlow8 = std::max<float>(-1e+02f, static_cast<float>(fHslider6));
		float fSlow9 = 0.01f * static_cast<float>(fHslider7);
		float fSlow10 = 1.0f / std::min<float>(std::max<float>(0.003f * fSlow0 + 0.5f, 0.001f), 0.999f) + -2.0f;
		for (int i0 = 0; i0 < count; i0 = faust_wrap_add(i0, 1)) {
			float fTemp0 = static_cast<float>(input0[i0]);
			fVec0[0] = fTemp0;
			fRec6[0] = -(fConst17 * (fConst18 * fRec6[1] - (fTemp0 + fVec0[1])));
			fRec5[0] = fRec6[0] - fConst16 * (fConst19 * fRec5[2] + fConst21 * fRec5[1]);
			fVec1[0] = fSlow3;
			float fTemp1 = static_cast<float>(fSlow3 <= fVec1[1]);
			float fTemp2 = 1.0f - fConst22 * fTemp1;
			fRec7[0] = fSlow2 * fTemp2 + fConst22 * fTemp1 * fRec7[1];
			fRec9[0] = -(fConst17 * (fConst18 * fRec9[1] - fConst15 * (fTemp0 - fVec0[1])));
			fRec8[0] = fRec9[0] - fConst16 * (fConst19 * fRec8[2] + fConst21 * fRec8[1]);
			fRec10[0] = fSlow4 * fTemp2 + fConst22 * fTemp1 * fRec10[1];
			int iTemp3 = fRec10[0] > 0.0f;
			float fTemp4 = fConst24 * std::pow(1e+01f, 0.05f * std::fabs(fRec10[0]));
			float fTemp5 = ((iTemp3) ? fConst24 : fTemp4);
			float fTemp6 = fConst13 * fRec4[1];
			float fTemp7 = 1.0f + fConst23 * (fConst23 + fTemp5);
			fRec4[0] = fConst16 * ((fRec5[2] + fRec5[0] + 2.0f * fRec5[1]) * std::pow(1e+01f, 0.05f * fRec7[0]) + fConst20 * (fRec8[2] + (fRec8[0] - 2.0f * fRec8[1]))) - (fRec4[2] * (1.0f + fConst23 * (fConst23 - fTemp5)) + fTemp6) / fTemp7;
			float fTemp8 = ((iTemp3) ? fTemp4 : fConst24);
			fRec11[0] = fSlow5 * fTemp2 + fConst22 * fTemp1 * fRec11[1];
			int iTemp9 = fRec11[0] > 0.0f;
			float fTemp10 = fConst26 * std::pow(1e+01f, 0.05f * std::fabs(fRec11[0]));
			float fTemp11 = ((iTemp9) ? fConst26 : fTemp10);
			float fTemp12 = fConst11 * fRec3[1];
			float fTemp13 = 1.0f + fConst25 * (fConst25 + fTemp11);
			fRec3[0] = (fTemp6 + fRec4[0] * (1.0f + fConst23 * (fConst23 + fTemp8)) + fRec4[2] * (1.0f + fConst23 * (fConst23 - fTemp8))) / fTemp7 - (fRec3[2] * (1.0f + fConst25 * (fConst25 - fTemp11)) + fTemp12) / fTemp13;
			float fTemp14 = ((iTemp9) ? fTemp10 : fConst26);
			fRec12[0] = fSlow6 * fTemp2 + fConst22 * fTemp1 * fRec12[1];
			int iTemp15 = fRec12[0] > 0.0f;
			float fTemp16 = fConst28 * std::pow(1e+01f, 0.05f * std::fabs(fRec12[0]));
			float fTemp17 = ((iTemp15) ? fConst28 : fTemp16);
			float fTemp18 = fConst8 * fRec2[1];
			float fTemp19 = 1.0f + fConst27 * (fConst27 + fTemp17);
			fRec2[0] = (fTemp12 + fRec3[0] * (1.0f + fConst25 * (fConst25 + fTemp14)) + fRec3[2] * (1.0f + fConst25 * (fConst25 - fTemp14))) / fTemp13 - (fRec2[2] * (1.0f + fConst27 * (fConst27 - fTemp17)) + fTemp18) / fTemp19;
			float fTemp20 = ((iTemp15) ? fTemp16 : fConst28);
			float fTemp21 = (fTemp18 + fRec2[0] * (1.0f + fConst27 * (fConst27 + fTemp20)) + fRec2[2] * (1.0f + fConst27 * (fConst27 - fTemp20))) / fTemp19;
			fVec2[0] = fTemp21;
			fRec1[0] = -(fConst5 * (fConst6 * fRec1[1] - (fTemp21 + fVec2[1])));
			fRec0[0] = fRec1[0] - fConst4 * (fConst29 * fRec0[2] + fConst31 * fRec0[1]);
			fRec14[0] = -(fConst5 * (fConst6 * fRec14[1] - fConst3 * (fTemp21 - fVec2[1])));
			fRec13[0] = fRec14[0] - fConst4 * (fConst29 * fRec13[2] + fConst31 * fRec13[1]);
			fRec15[0] = fSlow7 * fTemp2 + fConst22 * fTemp1 * fRec15[1];
			float fTemp22 = fConst4 * (fRec0[2] + fRec0[0] + 2.0f * fRec0[1] + fConst30 * (fRec13[2] + (fRec13[0] - 2.0f * fRec13[1])) * std::pow(1e+01f, 0.05f * fRec15[0]));
			float fTemp23 = std::fabs(fTemp22);
			fRec16[0] = fSlow8 * fTemp2 + fConst22 * fTemp1 * fRec16[1];
			float fTemp24 = fTemp23 * (1.0f - static_cast<float>(faust_wrap_mul(2, fTemp22 < 0.0f))) * std::pow(1e+01f, 0.05f * fRec16[0]);
			fRec17[0] = fSlow9 * fTemp2 + fConst22 * fTemp1 * fRec17[1];
			float fTemp25 = 1.0f + fSlow10 * (1.0f - fTemp23);
			output0[i0] = static_cast<FAUSTFLOAT>(fSlow1 * (fTemp24 * std::min<float>(1.0f, 1.0f - fRec17[0]) / fTemp25));
			output1[i0] = static_cast<FAUSTFLOAT>(fSlow1 * (fTemp24 * std::min<float>(1.0f, 1.0f + fRec17[0]) / fTemp25));
			fVec0[1] = fVec0[0];
			fRec6[1] = fRec6[0];
			fRec5[2] = fRec5[1];
			fRec5[1] = fRec5[0];
			fVec1[1] = fVec1[0];
			fRec7[1] = fRec7[0];
			fRec9[1] = fRec9[0];
			fRec8[2] = fRec8[1];
			fRec8[1] = fRec8[0];
			fRec10[1] = fRec10[0];
			fRec4[2] = fRec4[1];
			fRec4[1] = fRec4[0];
			fRec11[1] = fRec11[0];
			fRec3[2] = fRec3[1];
			fRec3[1] = fRec3[0];
			fRec12[1] = fRec12[0];
			fRec2[2] = fRec2[1];
			fRec2[1] = fRec2[0];
			fVec2[1] = fVec2[0];
			fRec1[1] = fRec1[0];
			fRec0[2] = fRec0[1];
			fRec0[1] = fRec0[0];
			fRec14[1] = fRec14[0];
			fRec13[2] = fRec13[1];
			fRec13[1] = fRec13[0];
			fRec15[1] = fRec15[0];
			fRec16[1] = fRec16[0];
			fRec17[1] = fRec17[0];
		}
	}

};

#endif
