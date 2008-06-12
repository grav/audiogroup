/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            quantizer.cc
 *
 *  Tue Jun 10 11:48:08 CEST 2008
 *  Copyright 2008 Bent Bisballe Nyeng
 *  deva@aasimon.org
 ****************************************************************************/

/*
 *  This file is part of DSPToolBox.
 *
 *  DSPToolBox is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  DSPToolBox is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with DSPToolBox; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA.
 */
#include "quantizer.h"
#include "config.h"

#include <stdio.h>

#include <cmath>

#define FRAME_SIZE 512

long double quant_sum = 0;

void quantize(float thres, float max[], int b, int s, samples_t *band)
{
  unsigned int quant = 31 * config::quality;
  if(max[b] > thres) {
    quant += (unsigned int)((32767-31) * (max[b]-thres)/max[b] * config::quality);
  }
  //printf("Quantizing ... (thres: %f, max: %f)\n",thres,max[b]);
  for(int t = 0; t < FRAME_SIZE; t++) {
    band->samples[s + t] = round(band->samples[s + t] * quant) / (float)quant;
  }
  quant_sum += FRAME_SIZE * ceil(log2(2*quant)) / 32;
  //printf("Quant: %f\t%d\n",(float)quant,b);
}

void poolQuantize(float thres,float max[], int b, int s, samples_t *band){
  
  float bps = 64*1024;
  float xfs = 44100;
  float fps = xfs/FRAME_SIZE;
  float bpf = bps/fps;

}
