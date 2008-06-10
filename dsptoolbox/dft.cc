/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            dft.cc
 *
 *  Wed Apr 30 14:14:29 CEST 2008
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
#include "dft.h"

/**
 * This code uses the fftw library.
 * Link: http://www.fftw.org/
 */
#include <fftw3.h>

complex_samples_t *dft(samples_t *samples)
{
  complex_samples_t *d = new complex_samples_t(samples->size);

  fftwf_plan p;
  p = fftwf_plan_dft_r2c_1d(samples->size, samples->samples, d->samples, FFTW_ESTIMATE| FFTW_PRESERVE_INPUT);
  fftwf_execute(p);

  fftwf_destroy_plan(p);

  return d;
}

samples_t *idft(complex_samples_t *samples)
{
  samples_t *i = new samples_t(samples->size);

  fftwf_plan p;
  p = fftwf_plan_dft_c2r_1d(samples->size, samples->samples, i->samples, FFTW_ESTIMATE| FFTW_PRESERVE_INPUT);
  fftwf_execute(p);

  fftwf_destroy_plan(p);

  for(int a = 0; a < i->size; a++) {
    i->samples[a] /= i->size;
  }

  return i;
}

