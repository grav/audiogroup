/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            wavwrite.cc
 *
 *  Wed Apr 30 13:42:29 CEST 2008
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
#include "wavwrite.h"

/**
 * This code uses libsndfile
 * Link: http://www.mega-nerd.com/libsndfile/
 */
#include <stdio.h>
#include <sndfile.h>

void wavwrite(const char *filename, samples_t *samples, samplerate_t samplerate)
{
  SF_INFO info;
  info.format = SF_FORMAT_WAV | SF_FORMAT_FLOAT;
  info.samplerate = samplerate;
  info.channels = 1;

  SNDFILE *handle = sf_open(filename, SFM_WRITE, &info);

  sf_writef_float(handle, samples->samples, samples->size);

  sf_close(handle);
}
