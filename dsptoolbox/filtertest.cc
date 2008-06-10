/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            filtertest.cc
 *
 *  Wed Apr 30 12:22:15 CEST 2008
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
#include "wavread.h"
#include "wavwrite.h"
#include "conv.h"
#include "wavplay.h"

#include <stdio.h>

int main(int argc, char *argv[])
{
  if(argc < 2) {
    printf("Usage %s input filter\n", argv[0]);
    return 1;
  }

  samplerate_t xfs;
  samples_t *x = wavread(argv[1], &xfs);

  samplerate_t hfs;
  samples_t *h = wavread(argv[2], &hfs);

  samples_t *c = conv(x, h);
  
  wavplay(c, xfs);

  delete x;

  return 0;
}
