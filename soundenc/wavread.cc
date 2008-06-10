/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            wavread.cc
 *
 *  Wed Apr 30 13:42:24 CEST 2008
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
#include "wavread.h"

/**
 * This code uses libsndfile
 * Link: http://www.mega-nerd.com/libsndfile/
 */
#include <stdio.h>
#include <sndfile.h>

samples_t *wavread(const char *filename, samplerate_t *samplerate)
{
  SF_INFO info;
  info.format = 0;

  SNDFILE *handle = sf_open(filename, SFM_READ, &info);

  if(samplerate) *samplerate = info.samplerate;

  samples_t *s = new samples_t(info.frames);

  sf_readf_float(handle, s->samples, s->size);

  sf_close(handle);

  return s;
}
