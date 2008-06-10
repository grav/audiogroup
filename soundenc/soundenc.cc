/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            soundenc.cc
 *
 *  Tue Jun 10 09:17:14 CEST 2008
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
#include "wavplay.h"
#include "wavwrite.h"
#include "conv.h"
#include "samples.h"
#include "normalize.h"
#include "threshold.h"

#include <stdio.h>
#include <string.h>
#include <math.h>

#include "filters.h"

#define NUM_BANDS 32
#define FRAME_SIZE 512

int main(int argc, char *argv[])
{
  if(argc < 2) {
    printf("Usage %s input\n", argv[0]);
    return 1;
  }

  samples_t *filters[NUM_BANDS];
  int f = 0;

  // Load filters
  printf("Loading filters...\n");
  char **filter = (char**)lin_filters;
  while(strlen(*filter)) {
    printf("Filter: %s\n", *filter);
    filters[f++] = wavread(*filter);
    filter++;
  }

  // Load input
  samplerate_t xfs;
  samples_t *x = wavread(argv[1], &xfs);

  // Split bands
  printf("Splitting bands...\n");
  samples_t *bands[NUM_BANDS];
  for(int b = 0; b < NUM_BANDS; b++) {
    printf("\r%d of %d ", b, NUM_BANDS); fflush(stdout);
    bands[b] = conv(x, filters[b]);
  }
  printf("\r%d of %d\n", NUM_BANDS, NUM_BANDS);

  float bandwidth = (20000.0-20.0)/32.0;
  // Iterate frames
  for(int b = 0; b < NUM_BANDS; b++) {
    float max = 0;
    for(int s = 0; s < x->size; s += FRAME_SIZE) {
      for(int t = 0; t < FRAME_SIZE; t++) {
        float sample = bands[b]->samples[s + t];
        if(fabs(sample) > max) max = fabs(sample);
      }
      int freq = (int)(b * bandwidth + bandwidth/2.0);
      float thres = threshold(freq);
      if(max < thres) {
        for(int t = 0; t < FRAME_SIZE; t++) {
          bands[b]->samples[s + t] = 0;
        }
      }
    }
  }


  samples_t y(x->size);
  // Put zeros in y.
  for(int s = 0; s < y.size; s++) {
    y.samples[s] = 0;
  }

  // Mix bands
  printf("Mixing bands...\n");
  for(int b = 0; b < NUM_BANDS; b++) {
    printf("\r%d of %d ", b, NUM_BANDS); fflush(stdout);
    for(int s = 0; s < y.size; s++) {
      y.samples[s] += bands[b]->samples[s];
    }
  }
  printf("\r%d of %d\n", NUM_BANDS, NUM_BANDS);

  normalize(&y);

  // Play result
  printf("Playing result...\n");
  // wavplay(&y, xfs);
  wavwrite("output.wav",&y, xfs);

  // Clean up
  delete x;

  for(int i = 0; i < NUM_BANDS; i++) {
    delete bands[i];
    delete filters[i];
  }

  return 0;
}
