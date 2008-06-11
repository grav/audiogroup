/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            biquadtest.cc
 *
 *  Wed Jun 11 09:16:33 CEST 2008
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
#include "biquad.h"
#include <stdio.h>

#define START 0
#define STOP 22
#define STEP 1

int main()
{
  biquad f;
  biquad_init(&f);

  bq_t fc = 20000;
  bq_t gain = 200;
  bq_t bw = 0.01;
  bq_t fs = 44100;
  eq_set_params(&f, fc, gain, bw, fs);

  printf("x = [");
  for(bq_t x = START; x < STOP; x+=STEP) {
    printf("%f ", x);
  }
  printf("];\n");

  printf("y = [");
  for(bq_t x = START; x < STOP; x+=STEP) {
    printf("%f ", biquad_run(&f, x));
  }  
  printf("];\n");

  return 0;
}
