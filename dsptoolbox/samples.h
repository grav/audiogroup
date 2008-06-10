/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            samples.h
 *
 *  Wed Apr 30 12:22:54 CEST 2008
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
#ifndef __DSPTOOLBOX_SAMPLES_H__
#define __DSPTOOLBOX_SAMPLES_H__

#include <unistd.h>

typedef float sample_t;

class samples_t {
public:
  samples_t(sample_t *samples, size_t size);
  samples_t(size_t size);
  ~samples_t();

  sample_t *samples;
  size_t size;
};

#define RE 0
#define IM 1
typedef float complex_sample_t[2];

class complex_samples_t {
public:
  complex_samples_t(complex_sample_t *samples, size_t size);
  complex_samples_t(size_t size);
  ~complex_samples_t();

  samples_t *re(double multiplier = 1.0);
  samples_t *im(double multiplier = 1.0);

  complex_sample_t *samples;
  size_t size;
};

typedef unsigned int samplerate_t;

#endif/*__DSPTOOLBOX_SAMPLES_H__*/
