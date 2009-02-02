/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            mls.cc
 *
 *  Mon Feb  2 18:45:42 CET 2009
 *  Copyright 2009 Bent Bisballe Nyeng
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
#include "mls.h"

/**
 * The code in this module has been taken from the wfir project:
 * http://www.ludd.luth.se/~torger
 *
 * (c) Copyright 1999 -- Anders Torger
 *
 * This software is free. You can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation.
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>

static int mls_taps[2][24][4] = {
    {{-1,-1,-1,-1},
     { 0, 1,-1,-1},
     { 1, 2,-1,-1},
     { 2, 3,-1,-1},
     { 2, 4,-1,-1},
     { 4, 5,-1,-1},
     { 5, 6,-1,-1},
     { 1, 2, 4, 7},
     { 4, 8,-1,-1},
     { 6, 9,-1,-1},
     { 8,10,-1,-1},
     { 5, 7,10,11},
     { 8, 9,11,12},
     { 3, 7,12,13},
     {13,14,-1,-1},
     { 3,12,14,15},
     {13,16,-1,-1},
     {10,17,-1,-1},
     {13,16,17,18},
     {16,19,-1,-1},
     {18,20,-1,-1},
     {20,21,-1,-1},
     {17,22,-1,-1},
     {16,21,22,23}},

    {{-1,-1,-1,-1},
     { 1, 0,-1,-1},
     { 2, 0,-1,-1},
     { 3, 0,-1,-1},
     { 4, 1,-1,-1},
     { 5, 0,-1,-1},
     { 6, 0,-1,-1},
     { 7, 6, 4, 0},
     { 8, 3,-1,-1},
     { 9, 6,-1,-1},
     {10, 1,-1,-1},
     {11, 6, 3, 2},
     {12, 3, 2, 0},
     {13,11,10, 0},
     {14, 7,-1,-1},
     {15, 4, 2, 1},
     {13,16,-1,-1},
     {17, 6,-1,-1},
     {18, 5, 4, 0},
     {19, 2,-1,-1},
     {20, 1,-1,-1},
     {21, 0,-1,-1},
     {22, 4,-1,-1},
     {23, 3, 2, 0}}
};

uint32_t *mls(unsigned int order, unsigned int tapset)
{
  int n, bitsum, ntaps, i, ti = order - 1;
  uint32_t *mls, p = 1;
    
  if (order > 24 || order == 0) {
    fprintf(stderr, "maximum_length_sequence: invalid order (%u). "
            "valid range is 1 - 24\n", order);
    return NULL;
  }
  if (tapset > 1) {
    fprintf(stderr, "maximum_length_sequence: invalid tap set (%u). "
            "valid range is 0 - 1\n", tapset);
    return NULL;
  }
  n = ((1 << order) / 8 < sizeof(uint32_t)) ? sizeof(uint32_t) :
    (1 << order) / 8;
  if ((mls = (uint32_t*)malloc(n)) == NULL) {
    fprintf(stderr, "maximum_length_sequence: could not allocate memory\n");
    return NULL;
  }
  bzero(mls, n);
  
  for (ntaps = 0; ntaps < 4 && mls_taps[tapset][ti][ntaps] != -1; ntaps++);
  for (n = 0; n < (1 << order) - 1; n++) {
    for (i = 0, bitsum = 0; i < ntaps; i++) {
      bitsum += (p >> (mls_taps[tapset][ti][i])) & 1;
    }
    p = (p << 1) + bitsum % 2;
    mls[n/32] = mls[n/32] | (p & 1) << (n % 32);
  }
  return mls;
}

