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
#include "biquadmask.h"

#include "biquad.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h> 

#include "samples.h"
#include "dft.h"
#include "normalize.h"
#include "ath.h"
#include "config.h"

#include <string.h>

#define STOP 200000

static float frand()
{
  double r = (double)rand() / (double)((unsigned int)0xffffffff);
  return r * 2.0 - 1.0;// * b + (b - a);
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

samples_t *rands = NULL;

float biquadmask(float max[], int band)
{
  samples_t *x = NULL;
  complex_samples_t *xfft = NULL;

  x = new samples_t(STOP / 100);
  if(rands == NULL) {
    rands = new samples_t(x->size);
    for(int i = 0; i < STOP / 100; i++) rands->samples[i] = frand() / NUM_BANDS;
  }

  memcpy(x->samples, rands->samples, x->size * sizeof(sample_t));

  for(int i = 0; i < NUM_BANDS; i++) {
    float c = ath(i);
    if(i != band && max[i] > c) {
      biquad f;
      biquad_init(&f);
      bq_t fc = band2freq(i) / 100;
      bq_t gain = max[i] - c;
      bq_t bw = 0.5;
      bq_t fs = 44100 / 100;
      eq_set_params(&f, fc, gain, bw, fs);
      for(int i = 0; i < STOP / 100; i++) {
        x->samples[i] = biquad_run(&f, x->samples[i]);
      }
    }
  }

  xfft = dft(x);

  float val = smooth(xfft, (int)band2freq(band));
  if(val < 0) val = 0;
  if(val > 1) val = 1;

  delete xfft;
  delete x;

  if(isinf(val) || isnan(val)) val = 0;

  return val;
}
