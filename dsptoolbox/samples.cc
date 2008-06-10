/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            samples.cc
 *
 *  Wed Apr 30 12:44:49 CEST 2008
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

samples_t::samples_t(sample_t *samples, size_t size)
{
  this->samples = samples;
  this->size = size;
}

samples_t::samples_t(size_t size)
{
  samples = new sample_t[size];
  this->size = size;
}

samples_t::~samples_t()
{
  delete[] samples; 
}

complex_samples_t::complex_samples_t(complex_sample_t *samples, size_t size)
{
  this->samples = samples;
  this->size = size;
}

complex_samples_t::complex_samples_t(size_t size)
{
  samples = new complex_sample_t[size];
  this->size = size;
}

complex_samples_t::~complex_samples_t()
{
  delete[] samples; 
}

samples_t *complex_samples_t::re(double multiplier)
{
  samples_t *r = new samples_t(size);
  for(int i = 0; i < size; i++) r->samples[i] = samples[i][RE] * multiplier;
  return r;
}

samples_t *complex_samples_t::im(double multiplier)
{
  samples_t *r = new samples_t(size);
  for(int i = 0; i < size; i++) r->samples[i] = samples[i][IM] * multiplier;
  return r;
}
