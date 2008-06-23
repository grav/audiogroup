/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            modulatetest.cc
 *
 *  Tue Jun 10 19:18:21 CEST 2008
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
#include "modulate.h"
#include "wavread.h"
#include "wavplay.h"
#include "normalize.h"

#include <cmath>
#include <stdio.h>

/*
sample_t mod_func(sample_t a)
{
  return a;// - 1000;
}
*/
int main(int argc, char *argv[])
{
  samplerate_t xfs;
  samples_t *x = wavread(argv[1], &xfs);

  //  printf("Original\n");
  //  wavplay(x, xfs);

  printf("Modulating\n");
  x = modulate(x, 300);
  printf("Playing\n");
  normalize(x);
  wavplay(x, xfs);

  printf("Modulating\n");
  x = modulate(x, 600);
  printf("Playing\n");
  normalize(x);
  wavplay(x, xfs);

  printf("Modulating\n");
  x = modulate(x, -1200);
  printf("Playing\n");
  normalize(x);
  wavplay(x, xfs);

  printf("Modulating\n");
  x = modulate(x, 300);
  printf("Playing\n");
  normalize(x);
  wavplay(x, xfs);

  return 0;
}
