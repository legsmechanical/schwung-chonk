/*
   ┏━━━┓╱╱╱╱╱┏┓╱╱┏┓╱╱╱╱╱┏┓╱╱
   ┃┏━┓┃╱╱╱╱╱┃┃╱╱┃┃╱╱╱╱╱┃┃╱╱╱
   ┃┗━┛┣┓┏┳━━┫┃┏┓┃┃╱╱┏━━┫┗━┳━━┓
   ┃┏━━┫┃┃┃┏┓┃┗┛┃┃┃╱┏┫┏┓┃┏┓┃━━┫
   ┃┃╱╱┃┗┛┃┃┃┃┏┓┃┃┗━┛┃┏┓┃┗┛┣━━┃
   ┗┛╱╱┗━━┻┛┗┻┛┗┛┗━━━┻┛┗┻━━┻━━┛
    ━━━━━━━━━━━━━━━━━━━━━━━━━━

    Copyright (c) 2024 Punk Labs LLC

    This section is part of OneTrick

    OneTrick is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the Free
    Software Foundation, either version 3 of the License, or (at your option)
    any later version.

    OneTrick is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
    more details.

    You should have received a copy of the GNU General Public License along with
    OneTrick.  If not, see <http://www.gnu.org/licenses/>.
*/

extern crate proc_macro;
use proc_macro::TokenStream;
use std::fs;
use syn::{
    parse_macro_input,
    DeriveInput,
    parse::Parser,
};
use quote::quote;


#[proc_macro]
pub fn include_dsp(path: TokenStream) -> TokenStream {

    let original_path = path.to_string();

    // Remove "quotes" from path, is there a better way to clean this?
    let path = path.to_string().replace("\"","");

    // Path needs to be adjusted to where dsp is usually imported...
    let path = format!("src/dsp/{}", path).to_string();
    // Maybe we can use concat!(env!("CARGO_MANIFEST_DIR"), "/").

    // Read the code...
    // WARNING: This doesn't notify compiler we're depending on path...
    //          We may be able to use syn::LitStr and include!()...
    let code = fs::read_to_string(path).expect("Failed to read DSP");

    let mut edited_code = "".to_string();
    let mut init_countdown = -1;
    // Hack to extend DSP struct with Extended features...
    for line in code.lines() {
        // Adds: Derive from FaustDspExtended
        // Adds: Extra Fields for FaustDspExtended
        if line.contains("pub struct") && !line.contains("SIG") { // "SIG0", "SIG1", etc are special Faust things...
            edited_code += "#[derive(FaustDspExtended)]\n";
            edited_code += "#[add_faust_dsp_extended_fields]\n";
        }
        edited_code += line;
        edited_code += "\n";

        // Adds: Imitialization for Extended Fields... (should find a better way...)
        // WARNING: This is never actually called! Is that our fault?
        // This causes issues where samples isn't initialized!
        // We could use instance_reset_params() if needed to ensure we're good...
        init_countdown -= 1;
        if line.contains("fn new()") {
            init_countdown = 1;
        }
        if init_countdown == 0 {
            edited_code += "\t\t\tsamples: Vec::new(),\n";
        }
    }


    // Transform code, adding extra features...
    // edited_code = edited_code.replace("pub struct", "#[derive(FaustDspExtended)]\n#[add_faust_dsp_extended_fields]\npub struct");
    edited_code = edited_code.replace("self__", "self.");
    // edited_code = edited_code.replace(self_tag, "self.wrapper.unwrap().dsp_");
    // edited_code = edited_code.replace("self__", "self.");
    // edited_code = edited_code.replace("Self__", "Self::");

    // println!("======================== EDITED CODE ========================\n{}", edited_code);

    // Hack to track changes to the DSP when rebuilding:
    // https://github.com/rust-lang/rust/issues/73921
    // Better solution in the works:
    // https://github.com/rust-lang/rust/issues/99515
    edited_code += format!("const _: &'static str = include_str!({});\n", original_path).as_str();

    // return parsed code
    edited_code.parse().unwrap()
}



// #[proc_macro_derive(FaustDspExtended)]
// pub fn derive_dsp_extended(_item: TokenStream) -> TokenStream {
//     "fn read_sample_nearest(&self, index: i32, time: f32) -> f32 { 0.0 }".parse().unwrap()
// }
#[proc_macro_derive(FaustDspExtended)]
pub fn derive(input: TokenStream) -> TokenStream {
    let DeriveInput { ident, .. } = parse_macro_input!(input);
    let output = quote! {
        impl FaustDspExtended for #ident {
            fn read_sample_raw(&self, index: i32, channel: i32, pos: i32, looping: i32) -> f32
            {
                if let Some(sample) = self.samples.get(index as usize) {
                    sample.read(channel as usize, pos as usize, looping != 0)
                } else {
                    0.0
                }
            }
            fn read_sample_nearest(&self, index: i32, channel: i32, time: f32, looping: i32) -> f32
            {
                if let Some(sample) = self.samples.get(index as usize) {
                    let sample_index = (time * sample.sample_rate() as f32) as usize;
                    sample.read(channel as usize, sample_index, looping != 0)
                } else {
                    0.0
                }
            }
            fn read_sample_linear(&self, index: i32, channel: i32, time: f32, looping: i32) -> f32
            {
                if let Some(sample) = self.samples.get(index as usize) {
                    let sample_index = (time * sample.sample_rate() as f32);
                    sample.read_linear(channel as usize, sample_index, looping != 0)
                } else {
                    0.0
                }
            }
            fn read_sample_cubic(&self, index: i32, channel: i32, time: f32, looping: i32) -> f32
            {
                if let Some(sample) = self.samples.get(index as usize) {
                    let sample_index = (time * sample.sample_rate() as f32);
                    sample.read_cubic(channel as usize, sample_index, looping != 0)
                } else {
                    0.0
                }
            }
            fn get_sample_rate(&self, index: i32) -> i32 {
                if let Some(sample) = self.samples.get(index as usize) {
                    sample.sample_rate() as i32
                } else {
                    0
                }
            }
            fn get_sample_duration(&self, index: i32) -> f32 {
                if let Some(sample) = self.samples.get(index as usize) {
                    // println!("Reading duration: {}", sample.duration());
                    sample.duration()
                } else {
                    // println!("No duration...");
                    0.0
                }
            }
            fn get_sample_channel_count(&self, index: i32) -> i32 {
                if let Some(sample) = self.samples.get(index as usize) {
                    sample.channel_count() as i32
                } else {
                    0
                }
            }
            fn get_sample_size(&self, index: i32) -> i32 {
                if let Some(sample) = self.samples.get(index as usize) {
                    sample.sample_count() as i32
                } else {
                    0
                }
            }
        
            fn set_sample_data(&mut self, index: usize, new_sample: FaustDspExtendedSample) {
                while self.samples.len() <= index {
                    self.samples.push(FaustDspExtendedSample::default());
                    // println!("adding... {}", self.samples.len());
                }
                // println!("setting... {}/{} = {}ch", index, self.samples.len(), data.len());
                if let Some(sample) = self.samples.get_mut(index) {
                    // println!("Some({})", sample.len());
                    *sample = new_sample;
                }
                // println!("DONE... ");
            }
        }
    };
    output.into()
}

// Append fields
#[proc_macro_attribute]
pub fn add_faust_dsp_extended_fields(_args: TokenStream, input: TokenStream) -> TokenStream  {
    let mut ast = parse_macro_input!(input as DeriveInput);
    match &mut ast.data {
        syn::Data::Struct(ref mut struct_data) => {           
            match &mut struct_data.fields {
                syn::Fields::Named(fields) => {
                    fields
                        .named
                        .push(syn::Field::parse_named.parse2(quote! {
                            samples: Vec<FaustDspExtendedSample>
                        }).unwrap());
                }   
                _ => {
                    ()
                }
            }              
            
            return quote! {
                #ast
            }.into();
        }
        _ => panic!("`add_faust_dsp_extended_fields` has to be used with structs "),
    }
}


// Might be nicer to simply append this at the end? But we'd still have to search for the classname...
// Append impl
// macro_rules! add_sampling_impl {
//     ($t:ident) => {
//         impl SamplingThing for $t {
//             fn read_sample_nearest(&self, t: f32) -> f32 {
//                 0.0
//             }
//         }
//     }
// }

// add_sampling_impl!(Point); // poof! `Point` now implements `Display`
