/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            biquadthreshold.cc
 *
 *  Wed Jun 11 12:21:47 CEST 2008
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
#include "biquadthreshold.h"

#include "biquad.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h> 

#include "samples.h"
#include "dft.h"
#include "normalize.h"

#include "config.h"

#define STOP 200000

static float frand()
{
  double r = (double)rand() / (double)((unsigned int)0xffffffff);
  return r * 2.0 - 1.0;// * b + (b - a);
}

static float _curve(int freq){
  // calculate from audibility threshold
  float thres = config::curve_offset;
  if (freq >= 0 && freq <= 4000) return thres + (-log10(freq) * 0.2775 + 1);
  else if ( freq > 4000 && freq <=20000 ) return thres + (0.0000625 * freq - 0.25);
  else return thres + 1;
}

static float curve(int freq){
  return fmax(0, _curve(freq));
}

static float smooth(complex_samples_t *x, int idx, int range = 100)
{
  float val = 0;//x->samples[idx][RE];
  int num = 0;
  float max = 0;
  for(int i = idx - range; i < idx + range; i++) {
    if(i >= 0 && i < x->size) {
      if(max < x->samples[i][RE]) max = x->samples[i][RE];
      val += x->samples[i][RE];
      num++;
    }  
  }
  val += max * range;

  val /= (float)(num * 3); 

  return val;
}

#define FRQ(x) ((20000 - 20) / 32) * x + (((20000 - 20) / 32) / 2)

float biquadthreshold(float max[], int band)
{
  samples_t *x = NULL;
  complex_samples_t *xfft = NULL;

  x = new samples_t(STOP);
  for(int i = 0; i < STOP; i++) x->samples[i] = frand() / 32;

  for(int i = 0; i < 32; i++) {
    float c = curve(FRQ(i));
    if(i != band && max[i] > c) {
      biquad f;
      biquad_init(&f);
      bq_t fc = FRQ(i);
      bq_t gain = fabs(c - max[i]) * 10;
      bq_t bw = 0.5;
      bq_t fs = 44100;
      //      printf("point ( %f %f %f)\n", fc, gain, bw);
      eq_set_params(&f, fc, gain, bw, fs);
      for(int i = 0; i < STOP; i++) {
        x->samples[i] = biquad_run(&f, x->samples[i]);
      }
    }
  }

  xfft = dft(x);

  float val = smooth(xfft, band * (20000/32));
  val /= 100;
  if(val < 0) val = 0;
  if(val > 1) val = 1;
  //  if(val < 0.3) val = 0;
  val = curve(FRQ(band)) + val;

  delete xfft;
  delete x;

  if(isinf(val)) val = 1.0 + config::curve_offset;
  //  printf("%f ", val); fflush(stdout);

  return val;
}
