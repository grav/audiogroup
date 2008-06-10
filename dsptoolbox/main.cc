/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            main.cc
 *
 *  Wed Apr 30 12:22:15 CEST 2008
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
#include "samples.h"
#include "resample.h"
#include "wavread.h"
#include "wavwrite.h"
#include "dft.h"
#include "dct.h"

int main()
{

  samplerate_t xfs;
  samples_t *x = wavread("in.wav", &xfs);

  samplerate_t yfs = xfs / 2;

  samples_t *s1 = resample(x, xfs, yfs);
  samples_t *s2 = resample(s1, yfs, xfs);

  complex_samples_t *s3 = dft(s2);
  samples_t *s4 = idft(s3);

  samples_t *s5 = resample(s4, xfs, 2000);

  samples_t *s6 = dct(s5);
  samples_t *s7 = idct(s6);

  samples_t *s8 = resample(s7, 2000, xfs);

  wavwrite("out.wav", s8, xfs);

  delete s1;
  delete s2;
  delete s3;
  delete s4;
  delete s5;
  delete s6;
  delete s7;
  delete s8;

  return 0;
}
