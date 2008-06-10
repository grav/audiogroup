/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            modulate.cc
 *
 *  Tue Jun 10 19:17:14 CEST 2008
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
#include "modulate.h"
#include <cmath>
#include <fftw3.h>

//samples_t *modulate(samples_t *x, modulation_function mod_func, float omega)
samples_t *modulate(samples_t *x, float omega)
{
  /*
  samples_t *y = new samples_t(x->size);
  for(int i = 0; i < x->size; i++) {
    //    y->samples[i] = x->samples[i] * cos(mod_func(i)*omega);
    y->samples[i] = (double)x->samples[i] * cos((double)i*(double)omega);
  }
  return y;
  */

  fftwf_plan p;

  samples_t *y = new samples_t(x->size);

  fftwf_complex *out = (fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex) * x->size);
  fftwf_complex *inx = (fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex) * x->size);
  fftwf_complex *xfft = (fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex) * x->size);

  for(int i = 0; i < x->size; i++) {
    inx[i][RE] = x->samples[i];
  }

  p = fftwf_plan_dft_1d(x->size, inx, xfft, FFTW_FORWARD, FFTW_ESTIMATE);
  fftwf_execute(p);
  /*
  for(int i = (int)omega; i < x->size - (int)omega; i++) {
    xfft[i][RE] = xfft[i + (int)omega][RE];// * cos(i*omega);
    xfft[i][IM] = xfft[i + (int)omega][IM];// * cos(i*omega);
  }
  */
#define DELTA ((int)omega)
  if(DELTA > 0) {
    for(int i = 0; i <= x->size - DELTA; i++) {
      xfft[i][RE] = xfft[i + DELTA][RE];
      xfft[i][IM] = xfft[i + DELTA][IM];
    }
    for(int i = x->size - DELTA; i < x->size; i++) {
      xfft[i][RE] = 0;
      xfft[i][IM] = 0;
    }
  } else {
    for(int i = x->size; i > -DELTA; i--) {
      xfft[i][RE] = xfft[i + DELTA][RE];
      xfft[i][IM] = xfft[i + DELTA][IM];
    }
    for(int i = 0; i <= -DELTA; i++) {
      xfft[i][RE] = 0;
      xfft[i][IM] = 0;
    }
  }

	p = fftwf_plan_dft_1d(x->size, xfft, out, FFTW_BACKWARD, FFTW_ESTIMATE);
  fftwf_execute(p);

  for(int i = 0; i < x->size; i++) {
    y->samples[i] = out[i][RE] / x->size;
  }

  fftwf_destroy_plan(p);

  fftwf_free(out); 
  fftwf_free(inx); 
  fftwf_free(xfft);

  return y;

}
