/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            dct.cc
 *
 *  Wed Apr 30 14:14:32 CEST 2008
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
#include "dct.h"

#include <math.h>
#include <stdio.h>

/**
 * This code is based on DCT-II and DCT-III from wikipedia
 * Link: http://en.wikipedia.org/wiki/Discrete_cosine_transform
 */

// DCT-II
samples_t *dct(samples_t *x)
{
  /*
          N-1
          ---            pi         1
    X_k = >   x_n cos [ ---- ( n + --- ) k ],      k = 0, ... , N-1.
          ---            N          2
          n=0
  */
  int N = x->size;
  samples_t *X = new samples_t(N);

  for(int k = 0; k <  N; k++) {
    X->samples[k] = 0;
    for(int n = 0; n < N; n++) {
      X->samples[k] += x->samples[n] * cos( (M_PI / (double)N) * ( n + 0.5) * k );
    }
    //    printf("\r%d of %d  ", k, N); fflush(stdout);
  }

  return X;
}


// DCT-III
samples_t *idct(samples_t *x)
{
  /*
                    N-1
           1        ---           pi           1
    X_k = --- x_0 + >   x_n cos[ ---- n ( k + --- ) ],    k = 0, ... , N-1.
           2        ---           N            2
                    n=1
  */
  int N = x->size;
  samples_t *X = new samples_t(N);

  for(int k = 0; k <  N; k++) {
    X->samples[k] = 0;

    for(int n = 1; n < N; n++) {
      X->samples[k] += x->samples[n] * cos( (M_PI / (double)N) * n * ( k + 0.5) );
    }

    X->samples[k] *= (0.5 * x->samples[0] ) / (double)N;

    //    printf("\r%d of %d  ", k, N); fflush(stdout);
  }

  return X;
}
