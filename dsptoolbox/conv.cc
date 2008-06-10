/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            conv.cc
 *
 *  Mon Jun  9 19:23:20 CEST 2008
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
#include "conv.h"

#include <fftw3.h>

samples_t *conv(samples_t *x, samples_t *h)
{
  fftwf_plan p;

  int size = x->size + h->size - 1;
  samples_t *y = new samples_t(size);

  fftwf_complex *outm = (fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex) * size);
  fftwf_complex *out = (fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex) * size);
  fftwf_complex *inx = (fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex) * size);
  fftwf_complex *inh = (fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex) * size);
  fftwf_complex *xfft = (fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex) * size);
  fftwf_complex *hfft = (fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex) * size);

  for(int i = 0; i < size; i++) {
    inx[i][RE] = inx[i][IM] = 0;
    inh[i][RE] = inh[i][IM] = 0;
  }

  for(int i = 0; i < x->size; i++) {
    inx[i][RE] = x->samples[i];
  }
  for(int i = 0; i < h->size; i++) {
    inh[i][RE] = h->samples[i];
  }

  p = fftwf_plan_dft_1d(size, inx, xfft, FFTW_FORWARD, FFTW_ESTIMATE);
  fftwf_execute(p);

  p = fftwf_plan_dft_1d(size, inh, hfft, FFTW_FORWARD, FFTW_ESTIMATE);
  fftwf_execute(p);

  for(int i = 0; i < size; i++ ) {
		outm[i][RE] = hfft[i][RE] * xfft[i][RE] - hfft[i][IM] * xfft[i][IM];
    outm[i][IM] = xfft[i][IM] * hfft[i][RE] + xfft[i][RE] * hfft[i][IM];
  }

	p = fftwf_plan_dft_1d(size, outm, out, FFTW_BACKWARD, FFTW_ESTIMATE);
  fftwf_execute(p);

  for(int i = 0; i < size; i++) {
    y->samples[i] = out[i][RE] / size;
  }

  fftwf_destroy_plan(p);

  fftwf_free(out); 
  fftwf_free(outm); 
  fftwf_free(inx); 
  fftwf_free(inh); 
  fftwf_free(xfft);
  fftwf_free(hfft);

  return y;
}
