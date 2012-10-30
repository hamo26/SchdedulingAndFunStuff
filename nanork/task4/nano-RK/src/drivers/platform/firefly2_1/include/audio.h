/******************************************************************************
*  Nano-RK, a real-time operating system for sensor networks.
*  Copyright (C) 2007, Real-Time and Multimedia Lab, Carnegie Mellon University
*  All rights reserved.
*
*  This is the Open Source Version of Nano-RK included as part of a Dual
*  Licensing Model. If you are unsure which license to use please refer to:
*  http://www.nanork.org/nano-RK/wiki/Licensing
*
*  This program is free software: you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation, version 2.0 of the License.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*  Contributing Authors (specific to this file):
*  Anthony Rowe
*******************************************************************************/


#ifndef AUDIO_H 
#define AUDIO_H 

#define AUDIO_BUFS   4
#define MAX_AUDIO_BUF 20 

uint8_t audio_buffers[AUDIO_BUFS][MAX_AUDIO_BUF];
uint8_t audio_cnt[AUDIO_BUFS];
uint8_t audio_index;
// Pins on the atmega128
#define MIC_PIN 3

void audio_sample(uint8_t state,uint8_t opt,uint8_t *buff,uint8_t size);
void audio_start();
void audio_init();
uint8_t audio_switch_buffers(); 
#endif
