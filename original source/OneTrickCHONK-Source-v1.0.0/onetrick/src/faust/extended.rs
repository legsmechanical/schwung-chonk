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
use wavers::{Wav, ReadSeek};
use std::io::{BufReader, Cursor};
use nih_plug::util::permit_alloc;


pub trait FaustDspExtended {
    fn read_sample_raw(&self, index: i32, channel: i32, pos: i32, looping: i32) -> f32;
    fn read_sample_nearest(&self, index: i32, channel: i32, time: f32, looping: i32) -> f32;
    fn read_sample_linear(&self, index: i32, channel: i32, time: f32, looping: i32) -> f32;
    fn read_sample_cubic(&self, index: i32, channel: i32, time: f32, looping: i32) -> f32;
    fn get_sample_duration(&self, index: i32) -> f32;
    fn get_sample_rate(&self, index: i32) -> i32;
    fn get_sample_channel_count(&self, index: i32) -> i32;
    fn get_sample_size(&self, index: i32) -> i32;
    fn set_sample_data(&mut self, index: usize, sample: FaustDspExtendedSample);
}

#[derive(Clone)]
pub struct FaustDspExtendedSample {
    data: Vec<Vec<f32>>,
    sample_rate: usize,
    duration: f32,
}

impl Default for FaustDspExtendedSample {
    fn default() -> Self {
        // Default to a single sample of stereo
        Self{
            data: [[0.0; 2].to_vec()].to_vec(),
            sample_rate: 44100,
            duration: 1.0,
        }
    }
}

impl FaustDspExtendedSample {
    pub fn new(data: Vec<Vec<f32>>, sample_rate: usize) -> Self {
        let duration = Self::calculate_duration(&data, sample_rate);
        Self{
            data,
            sample_rate,
            duration,
        }
    }

    pub fn from_wav_bytes(wav_bytes: &'static [u8]) -> Self{
        let mut array: Vec<Vec<f32>> = Vec::new();
        if let Some(list) = array.get_mut(1) {
            *list = Vec::new();
        }
        permit_alloc(|| {
            let reader = BufReader::new(Cursor::new(wav_bytes));
            let buf_reader: Box<dyn ReadSeek> = Box::new(reader);
            let mut data: Vec<Vec<f32>> = Vec::new();
            let mut sample_rate: usize = 44100;
            if let Ok(mut wav) = Wav::<f32>::new(buf_reader) {
                // let duration = wav.n_samples() as f32 / wav.sample_rate() as f32;
                sample_rate = wav.sample_rate() as usize;
                for channel in wav.channels() {
                    data.push(channel.to_vec());
                }
            }
            let duration = Self::calculate_duration(&data, sample_rate);
            Self{
                data,
                sample_rate,
                duration,
            }
        })
    }

    fn calculate_duration(data: &Vec<Vec<f32>>, sample_rate: usize) -> f32 {
        let sample_count = if let Some(channel) = data.get(0) {
            channel.len()
        } else {
            0
        };

        let duration = if sample_rate > 0 {
            sample_count as f32 / sample_rate as f32
        } else {
            1.0
        };
        duration
    }

    pub fn read(&self, channel: usize, index: usize, looping: bool) -> f32 {
        if let Some(channel_data) = self.data.get(channel as usize) {
            let index = if looping {index % channel_data.len()} else {index};
            if let Some(result) = channel_data.get(index) {
                return *result;
            }
        }
        0.0
    }

    pub fn read_linear(&self, channel: usize, index: f32, looping: bool) -> f32 {
        if let Some(channel_data) = self.data.get(channel as usize) {
            let mut index1 = index as usize;
            let mut index2 = index as usize + 1;
            if looping {
                let sample_count = channel_data.len();
                index1 = index1 % sample_count; // overly precaucious
                index2 = index2 % sample_count;
            }
            let value1 = if let Some(result) = channel_data.get(index1) {
                *result
            } else {
                0.0
            };
            let value2 = if let Some(result) = channel_data.get(index2) {
                *result
            } else {
                0.0
            };
            let fract = index - index.floor();
            return value1 + (value2-value1) * fract;
        }
        0.0
    }

    pub fn read_cubic(&self, channel: usize, index: f32, looping: bool) -> f32 {
        if let Some(channel_data) = self.data.get(channel as usize) {
            let mut index1 = index as usize;
            let mut index2 = index as usize + 1;
            let mut index3 = index as usize + 2;
            let mut index4 = index as usize + 3;
            if looping {
                let sample_count = channel_data.len();
                index1 = index1 % sample_count; // overly precaucious
                index2 = index2 % sample_count;
                index3 = index3 % sample_count;
                index4 = index4 % sample_count;
            }
            let value1 = if let Some(result) = channel_data.get(index1) {
                *result
            } else {
                0.0
            };
            let value2 = if let Some(result) = channel_data.get(index2) {
                *result
            } else {
                0.0
            };
            let value3 = if let Some(result) = channel_data.get(index3) {
                *result
            } else {
                0.0
            };
            let value4 = if let Some(result) = channel_data.get(index4) {
                *result
            } else {
                0.0
            };
            let fract = index - index.floor();
            return value2 + 0.5*fract*(value4 - value2 + fract*(2.0*value1 - 5.0*value2 + 4.0*value3 - value4 + fract*(3.0*(value2 - value3) + value4 - value1)));
        }
        0.0
    }
    pub fn sample_rate(&self) -> usize {
        self.sample_rate
    }

    pub fn duration(&self) -> f32 {
        self.duration
    }

    pub fn channel_count(&self) -> usize {
        self.data.len()
    }

    pub fn sample_count(&self) -> usize {
        if let Some(channel) = self.data.get(0) {
            channel.len()
        } else{
            0
        }
    }
}