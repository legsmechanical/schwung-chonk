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

declare name "OneTrick CHONK Output";
declare copyright "Copyright (c) 2024 Punk Labs LLC";
declare license "GPLv3 (or later)";

import("stdfaust.lib");
import("onetrick.lib");
import("params.lib");


addMixSaturation = ot.saturationUrchin(mixSaturation * 0.75); // Tone it down a bit...

band1Hz = 100;
band2Hz = 250;
band3Hz = 500;
band4Hz = 1500;
band5Hz = 3000;
eq(band1, band2, band3, band4, band5) = result with {
    result = _
        : fi.low_shelf(band1, band1Hz)
        : fi.peak_eq_cq(band2, band2Hz, 0.71)
        : fi.peak_eq_cq(band3, band3Hz, 0.71)
        : fi.peak_eq_cq(band4, band4Hz, 0.71)
        : fi.high_shelf(band5, band5Hz)
        ;
};
applyEQ = eq(eqBand1, eqBand2, eqBand3, eqBand4, eqBand5);

process(mono) = mono
        : applyEQ
        : addMixSaturation
        : *(mixGain)
        <: ot.stereoPanner(mixPan);

// Test for recreating filters
// process(mono) = mono <: si.bus(2);

