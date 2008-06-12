/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            ath.cc
 *
 *  Thu Jun 12 10:34:44 CEST 2008
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
#include "ath.h"
#include "config.h"

#include <cmath>

float ath_freq(float freq)
{
  float val = 1;
  // calculate from audibility threshold
  if (freq >= 20 && freq <= 4000) val = -log10(freq) * 0.2775 + 1;
  else if ( freq > 4000 && freq <= 20000 ) val = 0.0000625 * freq - 0.25;
  return fmaxf(0, val);
}

float band2freq(int band){
  switch(config::filter){
  case FILTER_LIN:
    {
      float bandwidth = (20000.0 - 20.0) / NUM_BANDS;
      return band * bandwidth + bandwidth/2.0;
    }
  case FILTER_LOG:
    {
      float k = powf((20000.0-20.0),(1.0/(float)NUM_BANDS));
      float lower = 20.0 + powf(k,(float)band);
      float upper = 20.0 + powf(k,(float)(band+1));
      return (upper-lower)/2.0;
    }
  }
}

float ath(int band){
  return ath_freq(band2freq(band));
}

