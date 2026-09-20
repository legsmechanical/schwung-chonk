┏━━━┓╱╱╱╱╱┏┓╱╱┏┓╱╱╱╱╱┏┓╱╱  
┃┏━┓┃╱╱╱╱╱┃┃╱╱┃┃╱╱╱╱╱┃┃╱╱╱  
┃┗━┛┣┓┏┳━━┫┃┏┓┃┃╱╱┏━━┫┗━┳━━┓  
┃┏━━┫┃┃┃┏┓┃┗┛┃┃┃╱┏┫┏┓┃┏┓┃━━┫  
┃┃╱╱┃┗┛┃┃┃┃┏┓┃┃┗━┛┃┏┓┃┗┛┣━━┃  
┗┛╱╱┗━━┻┛┗┻┛┗┛┗━━━┻┛┗┻━━┻━━┛  
━━━━━━━━━━━━━━━━━━━━━━━━━━

# OneTrick CHONK

A physically modeled electric bass monosynth.  
It is free as in rights, not as in beer.  
Please consider purchasing a copy to support our development efforts.

Website: <https://punklabs.com/ot-chonk>  
Contact: <contact@punklabs.com>

## Build Instructions

### Building the DSP (optional)
  
After installing the [Faust][Faust] compiler, you can build the DSP as follows:

#### Output DSP

    faust --check-table 0 --timeout 0 --process-name process -lang rust --architecture-dir onetrick/src/faust --import-dir onetrick/src/dsp -a arch.rs -o src/dsp/generated/dsp_output.rs --class-name DSP_Output -dlt 65536 src/dsp/output.dsp

#### Bass DSP

    faust --check-table 0 --timeout 0 --process-name process -lang rust --architecture-dir onetrick/src/faust --import-dir onetrick/src/dsp -a arch.rs -o src/dsp/generated/dsp_bass.rs --class-name DSP_Bass -dlt 65536 src/dsp/bass.dsp

### Building the plugin

After installing the [Rust][Rust] toolchain, you can build the VST3 and CLAP plugins as follows:

    cargo xtask bundle onetrick_chonk --release

To build the Audio Unit you will need the [Steinberg VST 3 SDK][vst3sdk] and the [Apple AudioUnit SDK][AudioUnitSDK]. Follow the instructions on building the [Audio Unit v2 Wrapper][auwrapper]. The `au-info.plist` is included in the `auwrapper` folder.

[Faust]: https://faust.grame.fr/

[Rust]: https://www.rust-lang.org/tools/install

[vst3sdk]: https://steinbergmedia.github.io/vst3_dev_portal/pages/Getting+Started/Links.html#getting-vst-3-sdk

[AudioUnitSDK]: https://github.com/apple/AudioUnitSDK

[auwrapper]: https://steinbergmedia.github.io/vst3_dev_portal/pages/What+is+the+VST+3+SDK/Wrappers/AUv2+Wrapper.html

## License

OneTrick CHONK is released under the [GNU General Public License v3.0][GPLv3] or Later (GPL-3.0-or-later). Please see `NOTICES.txt` for futher acknowledgments.

    OneTrick CHONK, a physically modeled electric bass monosynth.
    Copyright (C) 2024 Punk Labs LLC

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
    
[GPLv3]: https://www.gnu.org/licenses/gpl-3.0.en.html
