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

declare name "OneTrick CHONK Bass";
declare author "Punk Labs LLC";
declare copyright "2024 Punk Labs LLC";
declare version "1.0";

import("onetrick.lib");
import("params.lib");


// env = en.ar(20/1000, 150/1000, ot.input.trigger) * ot.input.velocity;
// osc = os.osc(ot.input.freq/2) * 0.5;
// bass = osc * env;

bass = result with {
    /*
        Low E: 41Hz
        High C on a 6 string 24th fret: 523
    */
    minNote = 24; // 28=Low E, 26=Drop D, 24=Drop C
    maxNote = 67; // High G, hitting waveguide limit w/ current arrangement...
    key = max(0, ot.input.key-24);
    trigger = ba.if((key>=minNote)*(key<=maxNote), ot.input.trigger, 0);
    // sizeBridgeToNut=89/100;
    minFreq = ba.midikey2hz(minNote);
    maxFreq = ba.midikey2hz(maxNote);
    maxSegmentLength = pm.f2l(minFreq);
    stringSegment(samples) = pm.waveguide(pm.l2s(maxSegmentLength), samples);

    slideKey = ot.timer.whileHeldScaledCumulative(articulationSlide!=0, ot.input.trigger, articulationSlide);

    // TODO: We shouldn't change bounceEfficienty, we should mute where we pick!
    //bounceEfficiencyOpen = 0.995;
    stringNote01 = stringNote : it.remap(minNote, maxNote, 0, 1);
    stringFreq01 = stringFreq : it.remap(ba.midikey2hz(minNote), ba.midikey2hz(maxNote), 0, 1);
    sustainTerm = it.interpolate_linear(sustain, 0.98, 1);
    bounceEfficiencyOpen = it.interpolate_linear(stringFreq01, 0.9945, 0.9999) * sustainTerm;
    bounceEfficiencyMuted = 0.5;
    articulationMuteTerm = it.interpolate_linear(articulationMute, 1, it.interpolate_linear(stringFreq01, 0.75, 0.95));
    bounceEfficiency = it.interpolate_linear(ot.input.gate : ot.smoothParamFast, bounceEfficiencyMuted,  bounceEfficiencyOpen*articulationMuteTerm) : ba.line(2/1000*ma.SR);

    stringThicknessDamping = stringNote : it.remap(minNote+6, maxNote-6, 0, 1) : max(0) : min(1) : it.interpolate_linear(_, 0.75, 1);
    stringCutoff = ot.interpolate_freq(stringBrightness, 8000, 12000) * stringThicknessDamping * (1-articulationMute*0.70);


    // stringFilter = fi.lowpass(1, stringCutoff);
    stringFilter = ot.filter.pinking(it.interpolate_linear(stringBrightness, 0.1, 0.04));
    
    //stringFilter = seq(i, 2, pm.bridgeFilter(stringBrightness * 0.5, 0));
    //stringFilter = fi.svf.lp(stringCutoff*1.27201964951, 0.57735026919);
    transientFilter = fi.highpass(1, 300) : fi.lowpass(1, 3750);
    bridgeFilter = stringFilter;
    nutFilter = stringFilter;
    toneFilter = fi.lowpass(1, toneKnob); // Passive bass: 1st order LPF

    shift = ot.input.pitchShiftSmooth(midiPitchWheelRange)+((ot.input.aftertouch*midiAftertouchRange) : ot.smoothParamFast);
    clampNote = max(minNote) : min(maxNote);
    quantizeFrets = _ <: it.interpolate_linear(articulationLegato+(abs(slideKey)>0.1), _, ot.quantizeFrets);
    stringSlide = ot.smoothT60_WithoutWake(articulationLegato*35/1000) : +(slideKey) : quantizeFrets;
    // stringSlide = ba.line(articulationLegato * 80/1000*ma.SR) : quantizeFrets;
    stringNote = key : stringSlide : +(shift) : clampNote : ot.smoothWaveguide; 
    stringFreq = ba.midikey2hz(stringNote) * fineTune; // Smooth prevents accidental pops/brightness
    stringLength = pm.f2l(stringFreq)/2;

    // Single pickup Fenders are at about 20% pos
    // P-Bass: High strings at 14%, Low at 18%
    // String length is 34in-35in or 86cm-89cm
    pickupPosCM = it.interpolate_linear(pickupPos01, 12, 20);
    pickupPos = it.interpolate_linear(stringNote01, pickupPosCM*3, pickupPosCM) / 100;


    pickSpeed = ot.input.velocity;
    fingerTravel01 = pickSpeed;
    fingerTravel01Pow = 1-(1-fingerTravel01)*(1-fingerTravel01);
    //fingerTravel01 = pow(pickSpeed, 0.75);
    //fingerTravel01 = 1-(1-pickSpeed)*(1-pickSpeed);
    fingerTravel = it.interpolate_linear(fingerTravel01, 0.2, 1.0);// * it.interpolate_linear(stringNote01, 1, 0.4); //String moves less at higher frequencies

    //fingerEnergy = ot.ar(it.interpolate_linear(pickSpeed, 1.5, 0.2)/1000, 20/1000, trigger) * fingerTravel : ot.filterTriggerClicks(trigger) : /(4);
    fingerEnergy = ot.stringPluck(stringFreq, fingerTravel/3, trigger);

    // TODO: Come up with a nicer exciter...
    // fingerEnergy = ot.ar(it.interpolate_linear(pickSpeed, 0.15, 0.02)/stringFreq, 1/stringFreq*4, trigger) * os.osc(stringFreq) * 0.25;


    clickVolume = pow(pickSpeed, 1.5);
    strikeHardnessAdjusted = it.interpolate_linear(articulationStyleAmount, strikeHardness, articulationStyleHardness) * clickVolume;
    transientEnv = ot.ar(0.2/1000, it.interpolate_linear(strikeHardnessAdjusted, 2, 8)/1000, trigger) * strikeHardnessAdjusted * 1.0;
    transientEnergy = ot.noise * transientEnv : transientFilter;

    bounce(efficiency, signal) = result with {
        // TODO: Nonlinear efficiency?
        result = signal * efficiency;
    };

    bridge = pm.lTermination(_
        : bounce(-bounceEfficiency)
        : bridgeFilter
        , pm.basicBlock);
    
    nut = pm.rTermination(pm.basicBlock, _
        : bounce(-bounceEfficiency)
        : nutFilter
        );
    

    //pickup(amount, x, y, s) = x, y, s+(x+y)*amount;
    pickup = pm.out;
    // excite = fingerEnergy + transientEnergy;
    // strike(pos01, excite) = (+(excite), +(excite:delay), _) with {
    //     fromPos = 0 / 100;
    //     toPos = 40 / 100;
    //     maxDelayS = pm.l2s(max(fromPos, toPos));
    //     delay = de.fdelay(maxDelayS, pm.l2s(it.interpolate_linear(pos01, fromPos, toPos)));
    // };
    //strike = pm.in; // Keep it simple
    strikePickup(thump, click, left, right, output) = (result_left, result_right, result_output) with {
        result_left = left + finalExcite;
        result_right = right + finalExcite;
        currentEnergy = (left+right):an.amp_follower(10/1000) :*(3);
        energyRequired01 = fingerTravel-currentEnergy : it.remap(-0.1, 0.1, 0, 1) : max(0) : min(1);
        appliedThump = thump * energyRequired01;
        appliedClick = click * energyRequired01*(1-articulationLegato);
        finalExcite = appliedThump+appliedClick;
        extraThump = it.interpolate_linear(articulationMute, mixThump, 1.5) * fingerTravel01Pow;
        result_output = left+right+(finalExcite+appliedClick+(appliedThump*extraThump))*(1-articulationLegato);
    };

    // If we need to limit the energy, we should do it in strike to reduce latency
    // limitString(left, right, out) = left+correction, right+correction, out with {
    //     total_energy = left+right;
    //     clamped_energy = total_energy : max(-1) : min(1);
    //     correction = (clamped_energy - total_energy) / 2;
    // };
    
    // This is an estimate from observation...
    butterworth1PoleLowpassCorrection(cutoff) = ((ma.log2(cutoff)-12)*0.333-0.75);
    // sampleCorrection = -4.5 + butterworth1PoleLowpassCorrection(stringCutoff)*2;
    pinkingCorrection(brightness) = brightness*0.3;
    sampleCorrection = -5.05 + pinkingCorrection(stringBrightness);

    output = result with {
        chain = pm.chain(
            bridge
            : stringSegment(pm.l2s(pickupPos))
            : strikePickup(fingerEnergy, transientEnergy) // Controllable
            : stringSegment(pm.l2s(stringLength)-pm.l2s(pickupPos) + sampleCorrection)
            : nut
        );
        result = pm.endChain(chain);
    };

    //spectral_tilt = fi.spectral_tilt(0, 20, 5000, -1/2);

    bassFix = fi.peak_eq_cq(6, 31, 0.5) : *(ba.db2linear(-3));

    result = output : ot.filterString : bassFix : toneFilter : ot.dcblocker; // : fi.highpass(4, 1000);
};


process = bass : si.bus(1);
