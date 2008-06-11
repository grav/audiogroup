/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            biquad.h
 *
 *  Wed Jun 11 09:16:40 CEST 2008
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
#ifndef __DSPTOOLBOX_BIQUAD_H__
#define __DSPTOOLBOX_BIQUAD_H__

#include <math.h>
#include <stdint.h>

/* 32 bit "pointer cast" union */
typedef union {
        float f;
        int32_t i;
} ls_pcast32;

static inline float flush_to_zero(float f)
{
	ls_pcast32 v;

	v.f = f;

	// original: return (v.i & 0x7f800000) == 0 ? 0.0f : f;
	// version from Tim Blechmann
	return (v.i & 0x7f800000) < 0x08000000 ? 0.0f : f;
}

#define LN_2_2 0.34657359f // ln(2)/2

#ifndef LIMIT
#define LIMIT(v,l,u) (v<l?l:(v>u?u:v))
#endif

#ifndef BIQUAD_TYPE
#define BIQUAD_TYPE float
#endif

typedef BIQUAD_TYPE bq_t;

/* Biquad filter (adapted from lisp code by Eli Brandt,
   http://www.cs.cmu.edu/~eli/) */

typedef struct {
	bq_t a1;
	bq_t a2;
	bq_t b0;
	bq_t b1;
	bq_t b2;
	bq_t x1;
	bq_t x2;
	bq_t y1;
	bq_t y2;
} biquad;

static inline void biquad_init(biquad *f) {
	f->x1 = 0.0f;
	f->x2 = 0.0f;
	f->y1 = 0.0f;
	f->y2 = 0.0f;
}

static inline void eq_set_params(biquad *f, bq_t fc, bq_t gain, bq_t bw,
			  bq_t fs)
{
	bq_t w = 2.0f * M_PI * LIMIT(fc, 1.0f, fs/2.0f) / fs;
	bq_t cw = cosf(w);
	bq_t sw = sinf(w);
	bq_t J = pow(10.0f, gain * 0.025f);
	bq_t g = sw * sinhf(LN_2_2 * LIMIT(bw, 0.0001f, 4.0f) * w / sw);
	bq_t a0r = 1.0f / (1.0f + (g / J));

	f->b0 = (1.0f + (g * J)) * a0r;
	f->b1 = (-2.0f * cw) * a0r;
	f->b2 = (1.0f - (g * J)) * a0r;
	f->a1 = -(f->b1);
	f->a2 = ((g / J) - 1.0f) * a0r;
}

static inline bq_t biquad_run(biquad *f, const bq_t x) {
	bq_t y;

	y = f->b0 * x + f->b1 * f->x1 + f->b2 * f->x2
		      + f->a1 * f->y1 + f->a2 * f->y2;
	y = flush_to_zero(y);
	f->x2 = f->x1;
	f->x1 = x;
	f->y2 = f->y1;
	f->y1 = y;

	return y;
}

#endif/*__DSPTOOLBOX_BIQUAD_H__*/
