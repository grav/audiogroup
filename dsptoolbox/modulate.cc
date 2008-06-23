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
samples_t *modulate(samples_t *x, int delta)
{
  fftwf_plan p;

  size_t size = x->size * 2;
  samples_t *y = new samples_t(x->size);

  fftwf_complex *out = (fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex) * size);
  fftwf_complex *inx = (fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex) * size);
  fftwf_complex *xfft = (fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex) * size);

  for(int i = 0; i < x->size; i++) {
    inx[i][RE] = x->samples[i];
  }

  p = fftwf_plan_dft_1d(size, inx, xfft, FFTW_FORWARD, FFTW_ESTIMATE);
  fftwf_execute(p);

  if(delta > 0) {
    for(int i = 0; i < size / 2; i++) {
      int offset = i + delta;
      if(offset >= 0 && offset < size / 2) {
        xfft[i][RE] = xfft[offset][RE];
        xfft[i][IM] = xfft[offset][IM];
      } else {
        xfft[i][RE] = 0;
        xfft[i][IM] = 0;
      }
    }
    for(int i = size; i > size / 2; i--) {
      int offset = i + delta;
      if(offset < size && offset >= size / 2) {
        xfft[i][RE] = xfft[offset][RE];
        xfft[i][IM] = xfft[offset][IM];
      } else {
        xfft[i][RE] = 0;
        xfft[i][IM] = 0;
      }
    }
  } else {
    for(int i = size / 2; i > 0; i--) {
      int offset = i + delta;
      if(offset < size / 2 && offset >= 0) {
        xfft[i][RE] = xfft[offset][RE];
        xfft[i][IM] = xfft[offset][IM];
      } else {
        xfft[i][RE] = 0;
        xfft[i][IM] = 0;
      }
    }
    for(int i = size / 2; i < size; i++) {
      int offset = i + delta;
      if(offset >= size / 2 && offset < size) {
        xfft[i][RE] = xfft[offset][RE];
        xfft[i][IM] = xfft[offset][IM];
      } else {
        xfft[i][RE] = 0;
        xfft[i][IM] = 0;
      }
    }
  }

	p = fftwf_plan_dft_1d(size, xfft, out, FFTW_BACKWARD, FFTW_ESTIMATE);
  fftwf_execute(p);

  for(int i = 0; i < y->size; i++) {
    y->samples[i] = out[i][RE] / y->size;
  }

  fftwf_destroy_plan(p);

  fftwf_free(out); 
  fftwf_free(inx); 
  fftwf_free(xfft);

  return y;

}
