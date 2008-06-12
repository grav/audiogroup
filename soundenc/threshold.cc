/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            threshold.cc
 *
 *  Tue Jun 10 10:02:53 CEST 2008
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
#include "threshold.h"
#include "config.h"
#include <cmath>
#include <stdio.h>

#include "biquadthreshold.h"
#include "ath.h"

float linthreshold(float max[], int band)
{
  float thres = ath(band);

  float left_diff = 0;
  float right_diff = 0;

  // look at neighbour bands
  if (band > 0){
    left_diff = fmaxf(0,max[band-1] - ath(band-1));
  }
  if (band < 31){
    right_diff = fmaxf(0,max[band+1] - ath(band+1));
  }
  float max_diff = fmaxf(left_diff,right_diff);

  thres += max_diff * config::mask_weight;
  return thres;

}

float avgthreshold(float max[], int band){
  float thres = ath(band);

  float left_diff = 0;
  float right_diff = 0;

  // look at neighbour bands
  if (band > 0){
    left_diff = fmaxf(0,max[band-1] - ath(band-1));
  }
  if (band < 31){
    right_diff = fmaxf(0,max[band+1] - ath(band+1));
  }
  float avg_diff = fabs(left_diff-right_diff);
  thres += avg_diff * config::mask_weight;
  return thres;

}


float threshold(float max[], int band)
{
  float mask = 0;
  switch(config::threshold) {
  case THR_LINEAR:
    mask = linthreshold(max, band);
    break;
  case THR_BIQUAD:
    mask = biquadthreshold(max, band);
    break;
  case THR_AVG:
    mask = avgthreshold(max, band);
    break;
  }
  return ath(band) + mask * config::mask_weight;
}
