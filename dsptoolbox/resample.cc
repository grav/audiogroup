/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            resample.cc
 *
 *  Wed Apr 30 12:22:08 CEST 2008
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
#include "resample.h"

#include <stdio.h>

/**
 * This code uses libsamplerate.
 * Link: http://www.mega-nerd.com/SRC/
 */
#include <samplerate.h>

samples_t *resample(samples_t *samples, samplerate_t sfrom, samplerate_t sto)
{
  double ratio = (double)sto / (double)sfrom;
  samples_t *res = new samples_t((size_t)((double)samples->size * ratio) + 1);

  SRC_DATA data;
  data.data_in = samples->samples;
  data.data_out = res->samples;
  data.input_frames = samples->size;
  data.output_frames = res->size;
  data.src_ratio = ratio;

  int err = src_simple(&data, SRC_SINC_BEST_QUALITY, 1);
  if(err) {
    fprintf(stderr, "Error in resample: %s\n", src_strerror(err));
  }

  res->size = data.output_frames_gen;

  // FIXME: Missing sample.
  // UGLY HACK. The last sample from the input is copied to the output.
  res->samples[res->size] = samples->samples[samples->size - 1];
  res->size++;

  return res;
}
